#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "s3d.h"

static inline float frand(){
    return ((float)rand())/(float)RAND_MAX;
}

int main(){

    srand(21082001);

    s3d_init_image(1920,1080);

    s3d_camera cam;
    cam.c = v3(0,0,-25);
    cam.fov = 90.0f;
    cam.theta = v3(0,0,0);
    s3d_update_camera(&cam);

    s3d_triangle tris[12];
    // front face
    tris[0].a = v3(0.5,0.5,-0.5);
    tris[0].b = v3(-0.5,-0.5,-0.5);
    tris[0].c = v3(-0.5,0.5,-0.5);
    tris[1].a = v3(0.5,0.5,-0.5);
    tris[1].b = v3(0.5,-0.5,-0.5);
    tris[1].c = v3(-0.5,-0.5,-0.5);

    // top face
    tris[2].a = v3(0.5,0.5,-0.5);
    tris[2].b = v3(-0.5,0.5,-0.5);
    tris[2].c = v3(0.5,0.5,0.5);
    tris[3].a = v3(-0.5,0.5,-0.5);
    tris[3].b = v3(-0.5,0.5,0.5);
    tris[3].c = v3(0.5,0.5,0.5);

    // bottom face
    tris[4].a = v3(0.5,-0.5,-0.5);
    tris[4].b = v3(0.5,-0.5,0.5);
    tris[4].c = v3(-0.5,-0.5,-0.5);
    tris[5].a = v3(-0.5,-0.5,-0.5);
    tris[5].b = v3(0.5,-0.5,0.5);
    tris[5].c = v3(-0.5,-0.5,0.5);

    // side face 1
    tris[6].a = v3(0.5,0.5,-0.5);
    tris[6].b = v3(0.5,0.5,0.5);
    tris[6].c = v3(0.5,-0.5,-0.5);
    tris[7].a = v3(0.5,-0.5,-0.5);
    tris[7].b = v3(0.5,0.5,0.5);
    tris[7].c = v3(0.5,-0.5,0.5);

    // side face 0.5
    tris[8].a = v3(-0.5,0.5,-0.5);
    tris[8].b = v3(-0.5,-0.5,-0.5);
    tris[8].c = v3(-0.5,0.5,0.5);
    tris[9].a = v3(-0.5,-0.5,-0.5);
    tris[9].b = v3(-0.5,-0.5,0.5);
    tris[9].c = v3(-0.5,0.5,0.5);

    // back face
    tris[10].a = v3(0.5,0.5,0.5);
    tris[10].b = v3(-0.5,0.5,0.5);
    tris[10].c = v3(-0.5,-0.5,0.5);
    tris[11].a = v3(0.5,0.5,0.5);
    tris[11].b = v3(-0.5,-0.5,0.5);
    tris[11].c = v3(0.5,-0.5,0.5);

    s3d_mesh mesh;
    mesh.n_tris = 12;
    mesh.tris = tris;
    for (int i = 0; i < 50; i++){
        printf(".\n");
        s3d_draw_mesh(cam,mesh,v3(frand() * 20.0f - 10.0f,frand() * 20.0f - 10.0f,frand() * 10 + 5),v3one(),v3(frand() * 2 * M_PI,frand() * 2 * M_PI,frand() * 2 * M_PI),SIMG_RED);
    }

    //s3d_draw_mesh(cam,mesh,v3zero(),v3one(),v3(0,0,0.2),SIMG_RED);

    /*s3d_triangle plane_tris[2];
    plane_tris[0].a = v3(-10,-2,-5);
    plane_tris[0].b = v3(-10,-2,20);
    plane_tris[0].c = v3(10,-2,20);
    plane_tris[1].a = v3(-10,-2,-5);
    plane_tris[1].b = v3(10,-2,20);
    plane_tris[1].c = v3(10,-2,-5);
    s3d_mesh plane_mesh;
    plane_mesh.n_tris = 2;
    plane_mesh.tris = plane_tris;
    s3d_draw_mesh(cam,plane_mesh,SIMG_WHITE);*/

    //s3d_mesh mesh2;
    //mesh2.n_tris = 6;
    //mesh2.tris = &tris[6];

    //s3d_draw_mesh(cam,mesh2,SIMG_GREEN);

    /*printf("%g %g\n",projected.x,projected.y);
    s3d_draw_pixel(cam,v3(1,1,1),SIMG_WHITE);
    s3d_draw_pixel(cam,v3(-1,1,1),SIMG_WHITE);
    s3d_draw_pixel(cam,v3(-1,-1,1),SIMG_WHITE);
    s3d_draw_pixel(cam,v3(1,-1,1),SIMG_WHITE);

    s3d_draw_pixel(cam,v3(1,1,2),SIMG_WHITE);
    s3d_draw_pixel(cam,v3(-1,1,2),SIMG_WHITE);
    s3d_draw_pixel(cam,v3(-1,-1,2),SIMG_WHITE);
    s3d_draw_pixel(cam,v3(1,-1,2),SIMG_WHITE);*/

    s3d_write_image("test.tga");

    s3d_destroy_image();

    return 0;
}