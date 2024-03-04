#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include <math.h>

typedef struct vec2{
    float x,y;
} vec2;

static inline vec2 v2(float x, float y){
    vec2 out;
    out.x = x;
    out.y = y;
    return out;
}

static inline vec2 v2v2add(vec2 l, vec2 r){
    vec2 out;
    out.x = l.x + r.x;
    out.y = l.y + r.y;
    return out;
}

static inline vec2 v2v2sub(vec2 l, vec2 r){
    vec2 out;
    out.x = l.x - r.x;
    out.y = l.y - r.y;
    return out;
}

static inline vec2 v2v2mul(vec2 l, vec2 r){
    vec2 out;
    out.x = l.x * r.x;
    out.y = l.y * r.y;
    return out;
}

static inline vec2 v2v2div(vec2 l, vec2 r){
    vec2 out;
    out.x = l.x / r.x;
    out.y = l.y / r.y;
    return out;
}

static inline float v2len2(vec2 l){
    return l.x*l.x + l.y*l.y;
}

static inline float v2len(vec2 l){
    return sqrtf(v2len2(l));
}

static inline vec2 v2fmul(vec2 l, float r){
    vec2 out;
    out.x = l.x * r;
    out.y = l.y * r;
    return out;
}

static inline vec2 v2zero(void){
    vec2 out;
    out.x = 0;
    out.y = 0;
    return out;
}

static inline vec2 v2one(void){
    vec2 out;
    out.x = 1;
    out.y = 1;
    return out;
}

#endif