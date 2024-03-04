#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "simg.h"
#include "simg_log.h"

struct pixel{
    unsigned char r,g,b;
};

struct image{
    int width, height;
    unsigned char* pixels;
};

static struct pixel make_pixel(simg_color col){
    struct pixel out;
    out.r = 255.0f * col.r;
    out.g = 255.0f * col.g;
    out.b = 255.0f * col.b;
    return out;
}

static simg_color get_color(struct pixel pixel){
    simg_color out;
    out.r = ((float)pixel.r)/255.0f;
    out.g = ((float)pixel.g)/255.0f;
    out.b = ((float)pixel.b)/255.0f;
    out.a = 1.0f;
    return out;
}

static struct image image;
static int image_initialized = 0;

int simg_init_image(int width, int height){
    assert(!image_initialized);

    log("initializing image with width = %d, height = %d",width,height);

    image.width = width;
    image.height = height;
    assert(image.pixels = malloc(sizeof(unsigned char) * width * height * 3));
    for (int i = 0; i < width*height; i++){
        struct pixel pixel = make_pixel(SIMG_BLACK);
        image.pixels[i * 3] = pixel.b;
        image.pixels[i * 3 + 1] = pixel.g;
        image.pixels[i * 3 + 2] = pixel.r;
    }

    image_initialized = 1;

    log("initialized image");

    return 0;
}

int simg_write_image(const char* fname){
    assert(image_initialized);

    log("writing image to %s",fname);

    static unsigned char tga[18] = {0};
    tga[2] = 2;
    tga[12] = 255 & image.width;
    tga[13] = 255 & (image.width >> 8);
    tga[14] = 255 & image.height;
    tga[15] = 255 & (image.height >> 8);
    tga[16] = 24;
    tga[17] = 32;

    FILE *write_ptr;
    write_ptr = fopen(fname,"wb");

    int success = !((1 == fwrite(tga, sizeof(tga), 1, write_ptr)) &&
           (1 == fwrite(image.pixels, sizeof(unsigned char) * image.width * image.height * 3, 1, write_ptr)));

    fclose(write_ptr);

    log("wrote image (success = %d)",success);

    return success;
}

int simg_destroy_image(void){
    assert(image_initialized);
    log("destroying image");
    image_initialized = 0;
    free(image.pixels);
    log("destroyed image");
    return 0;
}

int simg_write_pixel(int x, int y, simg_color col){
    assert(image_initialized);
    assert((x < image.width) && (x >= 0));
    assert((y < image.height) && (y >= 0));
    struct pixel pixel = make_pixel(simg_alpha_blend(col,simg_read_pixel(x,y)));
    image.pixels[(y * image.width + x) * 3] = pixel.b;
    image.pixels[(y * image.width + x) * 3 + 1] = pixel.g;
    image.pixels[(y * image.width + x) * 3 + 2] = pixel.r;
    return 0;
}

simg_color simg_read_pixel(int x, int y){
    assert(image_initialized);
    assert((x < image.width) && (x >= 0));
    assert((y < image.height) && (y >= 0));
    struct pixel pixel;
    pixel.r = image.pixels[(y * image.width + x)*3];
    pixel.g = image.pixels[(y * image.width + x)*3 + 1];
    pixel.b = image.pixels[(y * image.width + x)*3 + 2];
    return get_color(pixel);
}

simg_color simg_alpha_blend(simg_color c1, simg_color c2){
    float alpha_a = c1.a;
    float alpha_b = c2.a;
    float alpha_0 = alpha_a + alpha_b * (1.0f - alpha_a);
    float r_0 = (c1.r * alpha_a + c2.r * alpha_b * (1.0f - alpha_a)) / alpha_0;
    float g_0 = (c1.g * alpha_a + c2.g * alpha_b * (1.0f - alpha_a)) / alpha_0;
    float b_0 = (c1.b * alpha_a + c2.b * alpha_b * (1.0f - alpha_a)) / alpha_0;
    simg_color out;
    out.r = r_0;
    out.g = g_0;
    out.b = b_0;
    out.a = alpha_0;
    return out;
}

int simg_get_image_width(void){
    assert(image_initialized);
    return image.width;
}

int simg_get_image_height(void){
    assert(image_initialized);
    return image.height;
}