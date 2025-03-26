#ifndef _PTI_H_
#define _PTI_H_
#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#define FILEPATH "ascii_7_step_2.png"
#define ASCII_SYM_WIDTH 7
#define ASCII_SYM_HEIGHT 16
#define ASCII_COUNT 95

typedef struct image_matrix {
    int ** matrix;
    int height;
    int width;
} image_matrix;

typedef struct char_matrix {
    int ** matrix;
    int height;
    int width;
} char_matrix;

typedef struct ascii_matrix {
    int ch;
    image_matrix matrix;
} ascii_matrix;

void init_image_matrix(image_matrix * matrix, int height, int width);
void destroy_image_matrix(image_matrix * matrix);
void read_png_file(const char* filename, image_matrix * matrix);
ascii_matrix * create_ascii_matrices(image_matrix* ascii_image_matrix);
char_matrix * conv_ascii_matrix(image_matrix * image, ascii_matrix * ascii_matrices, int count, int height, int width, int scale);
#endif // _PTI_H_