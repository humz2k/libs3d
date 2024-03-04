#include <stdio.h>
#include "camera.h"

#define DEG2RAD 0.0174533

void s3d_update_camera(s3d_camera* cam){
    assert(cam);
    cam->rotation_matrix = s3d_get_rotation_matrix(cam->theta);
    cam->ez = 2 * atanf(1.0f/(cam->fov * DEG2RAD));
}

vec3 s3d_project(s3d_camera cam, vec3 a){
    vec3 d = m33v3mul(cam.rotation_matrix,v3v3sub(a,cam.c));
    return v3(
        ((cam.ez / d.z) * d.x + 1.0f) * 0.5f,
        ((cam.ez / d.z) * d.y + 1.0f) * 0.5f,
        d.z
    );
}