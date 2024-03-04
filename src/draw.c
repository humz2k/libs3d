#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "vector3.h"
#include "vector2.h"
#include "camera.h"
#include "simg.h"
#include "draw.h"

static float* zbuff;

int s3d_init_image(int width, int height){
    assert(zbuff = malloc(sizeof(float) * width * height));
    for (int i = 0; i < width * height; i++){
        zbuff[i] = 1000000.0f;
    }
    return simg_init_image(width,height);
}

int s3d_write_image(const char* fname){
    return simg_write_image(fname);
}

int s3d_destroy_image(void){
    int success = simg_destroy_image();
    free(zbuff);
    return success;
}

int s3d_draw_pixel(s3d_camera cam, vec3 point, simg_color color){
    vec3 projected = s3d_project(cam,point);
    int x = projected.x * simg_get_image_width();
    int y = projected.y * simg_get_image_width();
    return simg_write_pixel(x,y,color);
}

typedef struct projected_triangle{
    vec3 a,b,c;
    float area;
    float area_mul;
} ptri;

static inline float ptri_area(ptri tri){
    return 0.5f * (-tri.b.y * tri.c.x + tri.a.y * (-tri.b.x + tri.c.x) + tri.a.x * (tri.b.y - tri.c.y) + tri.b.x * tri.c.y);
}

static inline ptri ptri_get(s3d_camera cam, s3d_triangle tri){
    vec2 image_dims = v2(simg_get_image_width(),simg_get_image_height());
    ptri out;
    out.a = s3d_project(cam,tri.a);
    out.b = s3d_project(cam,tri.b);
    out.c = s3d_project(cam,tri.c);
    out.area = ptri_area(out);
    out.area_mul = 1.0f/(2.0f * out.area);
    return out;
}

static inline int ptri_barycenter(ptri tri, vec2 p){
    if (tri.area == 0)return 0;
    float s = tri.area_mul * (tri.a.y * tri.c.x - tri.a.x * tri.c.y + (tri.c.y - tri.a.y) * p.x + (tri.a.x - tri.c.x) * p.y);
    float t = tri.area_mul * (tri.a.x * tri.b.y - tri.a.y * tri.b.x + (tri.a.y - tri.b.y) * p.x + (tri.b.x - tri.a.x) * p.y);
    return (s >= 0) && (t >= 0) && ((1 - s - t) >= 0);
}

static inline vec3 get_inter_pos(ptri tri, vec2 p){
    float xv1 = tri.a.x;
    float yv1 = tri.a.y;
    float xv2 = tri.b.x;
    float yv2 = tri.b.y;
    float xv3 = tri.c.x;
    float yv3 = tri.c.y;
    float px = p.x;
    float py = p.y;
    vec3 w;
    w.x = ((yv2-yv3)*(px - xv3) + (xv3 - xv2)*(py-yv3))/((yv2-yv3)*(xv1-xv3) + (xv3 - xv2)*(yv1-yv3));
    w.y = ((yv3-yv1)*(px-xv3) + (xv1-xv3)*(py-yv3))/((yv2-yv3)*(xv1-xv3) + (xv3-xv2)*(yv1-yv3));
    w.z = (1 - w.x) - w.y;
    return w;
}

static inline float interpolate_zs(ptri tri, vec2 p){
    vec3 w = get_inter_pos(tri,p);
    float z = tri.a.z * w.x + tri.b.z * w.y + tri.c.z * w.z;
    return z;
}

static inline vec3 interpolate_world_pos(s3d_triangle world_tri, ptri tri, vec2 p){
    vec3 w = get_inter_pos(tri,p);
    float x = world_tri.a.x * w.x + world_tri.b.x * w.y + world_tri.c.x * w.z;
    float y = world_tri.a.y * w.x + world_tri.b.y * w.y + world_tri.c.y * w.z;
    float z = world_tri.a.z * w.x + world_tri.b.z * w.y + world_tri.c.z * w.z;
    return v3(x,y,z);
}

static int check_zbuff(int x, int y, float z, int set){
    float z0 = zbuff[y * simg_get_image_width() + x];
    if (z < z0){
        if (set)
            zbuff[y * simg_get_image_width() + x] = z;
        return 1;
    }
    return 0;
}

struct light{
    float ka;
    float kd;
    float ks;
    float p;
    float intensity;
    vec3 pos;
    vec3 cL;
};

#define MAX(a,b) ((a > b) ? a : b)

struct bounding_box{
    int xmin,xmax,ymin,ymax;
};

static struct bounding_box get_bounding_box(ptri projected){
    int width = simg_get_image_width();
    int height = simg_get_image_height();
    float fwidth = width;
    float fheight = height;

    float min_a_x = floorf(projected.a.x * fwidth);
    float min_b_x = floorf(projected.b.x * fwidth);
    float min_c_x = floorf(projected.c.x * fwidth);

    float min_x = min_a_x;
    if (min_b_x < min_x){
        min_x = min_b_x;
    }
    if (min_c_x < min_x){
        min_x = min_c_x;
    }
    min_x -= 5;
    if (min_x < 0){
        min_x = 0;
    }

    float max_a_x = ceilf(projected.a.x * fwidth);
    float max_b_x = ceilf(projected.b.x * fwidth);
    float max_c_x = ceilf(projected.c.x * fwidth);

    float max_x = max_a_x;
    if (max_b_x > max_x){
        max_x = max_b_x;
    }
    if (max_c_x > max_x){
        max_x = max_c_x;
    }
    max_x += 5;
    if (max_x > width){
        min_x = width;
    }

    float min_a_y = floorf(projected.a.y * fheight);
    float min_b_y = floorf(projected.b.y * fheight);
    float min_c_y = floorf(projected.c.y * fheight);

    float min_y = min_a_y;
    if (min_b_y < min_y){
        min_y = min_b_y;
    }
    if (min_c_y < min_y){
        min_y = min_c_y;
    }
    min_y -= 5;
    if (min_y < 0){
        min_y = 0;
    }

    float max_a_y = ceilf(projected.a.y * fheight);
    float max_b_y = ceilf(projected.b.y * fheight);
    float max_c_y = ceilf(projected.c.y * fheight);

    float max_y = max_a_y;
    if (max_b_y > max_y){
        max_y = max_b_y;
    }
    if (max_c_y > max_y){
        max_y = max_c_y;
    }
    max_y += 5;
    if (max_y > height){
        max_y = height;
    }

    struct bounding_box out;
    out.xmin = 0;
    out.xmax = width;
    out.ymin = 0;
    out.ymax = height;
    return out;

}

int s3d_draw_triangle(s3d_camera cam, s3d_triangle tri, simg_color color){

    struct light light;
    light.ka = 0.1;
    light.kd = 0.5;
    light.ks = 0.4;
    light.p = 200;
    light.pos = cam.c;
    light.intensity = 200.0f;
    light.cL = v3one();

    ptri projected = ptri_get(cam,tri);

    int width = simg_get_image_width();
    int height = simg_get_image_height();

    struct bounding_box bbox = get_bounding_box(projected);

    vec3 N = v3norm(v3v3cross(v3v3sub(tri.a,tri.b),v3v3sub(tri.a,tri.c)));
    float diff = ((float)(height - width)) * 0.5f;
    for (int x = bbox.xmin; x < bbox.xmax; x++){
        for (int y = bbox.ymin; y < bbox.ymax; y++){
            vec2 p = v2((float)x/(float)width,((float)y - diff)/(float)width);
            int inside = ptri_barycenter(projected,p);
            int count = 1;
            float mul = 1;
            /*if (!inside){
                int subsample = 2;
                count = 0;
                for (int i = -1; i < 2; i++){
                    for (int j = -1; j < 2; j++){
                        vec2 ps = v2((((float)x) + 0.2f * (float)i)/(float)width,((((float)y) - diff) + 0.2f * (float)i)/(float)width);
                        inside += ptri_barycenter(projected,ps);
                        count++;
                    }
                }

                mul = ((float)inside)/(float)count;
            }*/

            /*int subsample = 4;
            float dx = (1.0f/(float)width) / (((float)subsample));
            float x_s = (((float)x) - 0.5f)/(float)width;
            float dy = (1.0f/(float)height) / (((float)subsample));
            float y_start = (((float)y) - 0.5f)/(float)height;
            int inside = 0;
            for (int i = 0; i < subsample; i++){
                float y_s = y_start;
                for (int j = 0; j < subsample; j++){
                    vec2 p_s = v2(x_s,y_s);
                    inside += ptri_barycenter(projected,p_s);
                    y_s += dy;
                }
                x_s += dx;
            }*/
            //if (ptri_barycenter(projected,p)){
            if (inside){
                float z = interpolate_zs(projected,p);
                if (check_zbuff(x,y,z,inside == count)){
                    vec3 wpos = interpolate_world_pos(tri,projected,p);
                    vec3 V = v3norm(v3v3sub(cam.c,wpos));
                    vec3 L = v3norm(v3v3sub(light.pos,wpos));
                    float dist2 = v3len2(v3v3sub(light.pos,wpos));
                    vec3 H = v3norm(v3v3add(V,L));
                    vec3 cM = v3(color.r,color.g,color.b);
                    vec3 ambient = v3fmul(cM,light.ka);
                    vec3 diffuse = v3fmul(v3v3mul(cM,light.cL),light.kd * MAX(0,v3v3dot(N,L)));
                    vec3 specular = v3fmul(light.cL,light.ks * (powf(MAX(0,v3v3dot(N,H)),light.p)));
                    vec3 out_col = v3clamp(v3v3add(ambient,v3fmul(v3v3add(diffuse,specular),light.intensity/dist2)),0,1);
                    simg_color col = color;
                    col.r = out_col.x;
                    col.g = out_col.y;
                    col.b = out_col.z;
                    col.a *= mul;//((float)inside) / (float)((subsample + 1) * (subsample + 1));

                    simg_write_pixel(x,height - (y+1),col);
                }
            }
        }
    }

    return 0;

}

int s3d_draw_mesh(s3d_camera cam, s3d_mesh mesh, vec3 position, vec3 scale, vec3 rotation, simg_color color){
    for (int i = 0; i < mesh.n_tris; i++){
        s3d_triangle tri = mesh.tris[i];
        mat3x3 mat = s3d_get_rotation_matrix(rotation);
        tri.a = m33v3mul(mat,v3v3mul(v3v3add(tri.a,position),scale));
        tri.b = m33v3mul(mat,v3v3mul(v3v3add(tri.b,position),scale));
        tri.c = m33v3mul(mat,v3v3mul(v3v3add(tri.c,position),scale));
        s3d_draw_triangle(cam,tri,color);
    }
    return 0;
}