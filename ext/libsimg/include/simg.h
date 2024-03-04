#ifndef _SIMG_H_
#define _SIMG_H_

typedef struct simg_color{
    float r,g,b,a;
} simg_color;

#define SIMG_WHITE ((simg_color){1.0f,1.0f,1.0f,1.0f})
#define SIMG_BLACK ((simg_color){0.0f,0.0f,0.0f,1.0f})
#define SIMG_GREEN ((simg_color){0.0f,1.0f,0.0f,1.0f})
#define SIMG_RED ((simg_color){1.0f,0.0f,0.0f,1.0f})
#define SIMG_BLUE ((simg_color){0.0f,0.0f,1.0f,1.0f})

int simg_init_image(int width, int height);
int simg_write_image(const char* fname);
int simg_destroy_image(void);
int simg_write_pixel(int x, int y, simg_color col);
simg_color simg_read_pixel(int x, int y);
simg_color simg_alpha_blend(simg_color c1, simg_color c2);
int simg_get_image_width(void);
int simg_get_image_height(void);

#endif