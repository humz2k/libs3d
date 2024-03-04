#include <stdlib.h>
#include <stdio.h>
#include "simg.h"

int main(){

    simg_init_image(1000,500);

    for (int x = 0; x < simg_get_image_width(); x++){
        for (int y = 0; y < simg_get_image_height(); y++){
            simg_color col;
            col.b = 0.0f;
            //col.g = 0.0f;
            col.r = 1.0f;
            col.g = ((float)y) / (float)simg_get_image_height();
            col.a = 1.0f;
            col.a = ((float)x) / (float)simg_get_image_width();
            simg_write_pixel(x,y,col);
        }
    }

    simg_write_image("test.tga");
    simg_destroy_image();

    return 0;
}