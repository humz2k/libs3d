#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <math.h>
#include <assert.h>
#include "vector3.h"
#include "vector2.h"
#include "matrix3x3.h"

typedef struct s3d_camera{
    vec3 c;
    float fov;
    float ez;
    vec3 theta;
    mat3x3 rotation_matrix;
} s3d_camera;

void s3d_update_camera(s3d_camera* cam);

vec3 s3d_project(s3d_camera cam, vec3 a);

#endif