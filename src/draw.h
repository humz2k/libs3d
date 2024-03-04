#ifndef _S3D_DRAW_H_
#define _S3D_DRAW_H_

#include "camera.h"
#include "vector3.h"
#include "simg.h"

typedef struct s3d_triangle{
    vec3 a, b, c;
} s3d_triangle;

typedef struct s3d_mesh{
    int n_tris;
    s3d_triangle* tris;
} s3d_mesh;

int s3d_init_image(int width, int height);

int s3d_write_image(const char* fname);

int s3d_destroy_image(void);

int s3d_draw_pixel(s3d_camera cam, vec3 point, simg_color color);

int s3d_draw_triangle(s3d_camera cam, s3d_triangle tri, simg_color color);

int s3d_draw_mesh(s3d_camera cam, s3d_mesh mesh, vec3 position, vec3 scale, vec3 rotation, simg_color color);

#endif