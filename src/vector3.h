#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <math.h>

typedef struct vec3{
    float x,y,z;
} vec3;

static inline vec3 v3(float x, float y, float z){
    vec3 out;
    out.x = x;
    out.y = y;
    out.z = z;
    return out;
}

static inline vec3 v3v3add(vec3 l, vec3 r){
    vec3 out;
    out.x = l.x + r.x;
    out.y = l.y + r.y;
    out.z = l.z + r.z;
    return out;
}

static inline vec3 v3v3sub(vec3 l, vec3 r){
    vec3 out;
    out.x = l.x - r.x;
    out.y = l.y - r.y;
    out.z = l.z - r.z;
    return out;
}

static inline vec3 v3v3mul(vec3 l, vec3 r){
    vec3 out;
    out.x = l.x * r.x;
    out.y = l.y * r.y;
    out.z = l.z * r.z;
    return out;
}

static inline vec3 v3v3div(vec3 l, vec3 r){
    vec3 out;
    out.x = l.x / r.x;
    out.y = l.y / r.y;
    out.z = l.z / r.z;
    return out;
}

static inline float v3len2(vec3 l){
    return l.x*l.x + l.y*l.y + l.z*l.z;
}

static inline float v3len(vec3 l){
    return sqrtf(v3len2(l));
}

static inline vec3 v3fmul(vec3 l, float r){
    vec3 out;
    out.x = l.x * r;
    out.y = l.y * r;
    out.z = l.z * r;
    return out;
}

static inline vec3 v3norm(vec3 v){
    return v3fmul(v,1.0f/v3len(v));
}

static inline vec3 v3zero(void){
    vec3 out;
    out.x = 0;
    out.y = 0;
    out.z = 0;
    return out;
}

static inline vec3 v3one(void){
    vec3 out;
    out.x = 1;
    out.y = 1;
    out.z = 1;
    return out;
}

static inline float v3v3dot(vec3 l, vec3 r){
    return l.x * r.x + l.y * r.y + l.z * r.z;
}

static inline vec3 v3sin(vec3 v){
    vec3 out;
    out.x = sinf(v.x);
    out.y = sinf(v.y);
    out.z = sinf(v.z);
    return out;
}

static inline vec3 v3cos(vec3 v){
    vec3 out;
    out.x = cosf(v.x);
    out.y = cosf(v.y);
    out.z = cosf(v.z);
    return out;
}

static inline vec3 v3clamp(vec3 v, float min, float max){
    vec3 out;
    out.x = v.x;
    if (out.x < min){
        out.x = min;
    }
    if (out.x > max){
        out.x = max;
    }

    out.y = v.y;
    if (out.y < min){
        out.y = min;
    }
    if (out.y > max){
        out.y = max;
    }

    out.z = v.z;
    if (out.z < min){
        out.z = min;
    }
    if (out.z > max){
        out.z = max;
    }

    return out;
}

static inline vec3 v3v3cross(vec3 l, vec3 r){
    return v3(
        l.y * r.z - l.z * r.y,
        l.z * r.x - l.x * r.z,
        l.x * r.y - l.y * r.x
    );
}

#endif