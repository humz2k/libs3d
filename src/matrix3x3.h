#ifndef _MATRIX3X3_H_
#define _MATRIX3X3_H_

#include <math.h>
#include "vector3.h"

typedef struct mat3x3{
    float m00, m01, m02;
    float m10, m11, m12;
    float m20, m21, m22;
} mat3x3;

static inline mat3x3 m33(float m00, float m01, float m02,
                        float m10, float m11, float m12,
                        float m20, float m21, float m22){
    mat3x3 out;
    out.m00 = m00;
    out.m01 = m01;
    out.m02 = m02;
    out.m10 = m10;
    out.m11 = m11;
    out.m12 = m12;
    out.m20 = m20;
    out.m21 = m21;
    out.m22 = m22;
    return out;
}

static inline vec3 m33row(mat3x3 m, int i){
    switch (i){
        case 0:
            return v3(m.m00,m.m01,m.m02);
        case 1:
            return v3(m.m10,m.m11,m.m12);
        case 2:
            return v3(m.m20,m.m21,m.m22);
        default:
            return v3zero();
    }
}

static inline vec3 m33col(mat3x3 m, int i){
    switch (i){
        case 0:
            return v3(m.m00,m.m10,m.m20);
        case 1:
            return v3(m.m01,m.m11,m.m21);
        case 2:
            return v3(m.m02,m.m12,m.m22);
        default:
            return v3zero();
    }
}

static inline mat3x3 m33m33mul(mat3x3 l, mat3x3 r){
    vec3 l_rows[3];
    for (int i = 0; i < 3; i++){
        l_rows[i] = m33row(l,i);
    }
    vec3 r_cols[3];
    for (int i = 0; i < 3; i++){
        r_cols[i] = m33col(r,i);
    }
    return m33(
        v3v3dot(l_rows[0],r_cols[0]), v3v3dot(l_rows[0],r_cols[1]), v3v3dot(l_rows[0],r_cols[2]),
        v3v3dot(l_rows[1],r_cols[0]), v3v3dot(l_rows[1],r_cols[1]), v3v3dot(l_rows[1],r_cols[2]),
        v3v3dot(l_rows[2],r_cols[0]), v3v3dot(l_rows[2],r_cols[1]), v3v3dot(l_rows[2],r_cols[2])
    );
}

static inline vec3 m33v3mul(mat3x3 l, vec3 r){
    return v3(v3v3dot(m33row(l,0),r),v3v3dot(m33row(l,1),r),v3v3dot(m33row(l,2),r));
}

static inline mat3x3 s3d_get_rotation_matrix(vec3 theta){
    vec3 sin_theta = v3sin(theta);
    vec3 cos_theta = v3cos(theta);
    mat3x3 rx = m33(
        1, 0, 0,
        0, cos_theta.x, sin_theta.x,
        0, -sin_theta.x, cos_theta.x
    );
    mat3x3 ry = m33(
        cos_theta.y, 0, -sin_theta.y,
        0,1,0,
        sin_theta.y,0,cos_theta.y
    );
    mat3x3 rz = m33(
        cos_theta.z, sin_theta.z, 0,
        -sin_theta.z,cos_theta.z,0,
        0,0,1
    );
    return m33m33mul(rx,m33m33mul(ry,rz));
}

#endif