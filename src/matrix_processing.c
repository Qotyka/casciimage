#include "pti.h"

int conv_int_matrices(int ** m1, int ** m2, int height, int width) {
    int result = 0;
    for(int i = 0; i < height; i ++)
        for(int j = 0; j < width; j ++) {
            result += m1[i][j] * m2[i][j];
        }
    return result;
}

void matrix_cpy(image_matrix * m1, image_matrix * m2, int index_x, int index_y, int height, int width) {
    for(int i = index_y; i < index_y + height; i ++) {
        for(int j = index_x; j < index_x + width; j ++) {
            (m1->matrix)[i - index_y][j - index_x] = (m2->matrix)[i][j];
        }
    }
}

ascii_matrix * create_ascii_matrices(image_matrix* ascii_image_matrix) {
    ascii_matrix * matrices = (ascii_matrix *) malloc(sizeof(ascii_matrix) * ASCII_COUNT);
    for(int i = 0; i < ASCII_COUNT; i++) {
        (matrices[i]).ch = i + 32;
        init_image_matrix(&((matrices[i]).matrix), ASCII_SYM_HEIGHT, ASCII_SYM_WIDTH);
        matrix_cpy(&((matrices[i]).matrix), ascii_image_matrix, i * ASCII_SYM_WIDTH, 0, ASCII_SYM_HEIGHT, ASCII_SYM_WIDTH);
    }
    return matrices;
}

char_matrix * conv_ascii_matrix(image_matrix * image, ascii_matrix * ascii_matrices, int count, int height, int width, int scale) {
    int result_height = scale * image->height / height;
    int result_width = scale * image->width / width;
    int ** result_int_matrix = (int**) malloc(sizeof(int*) * result_height);
    for(int i = 0; i < result_height; i++)
        result_int_matrix[i] = (int*) malloc(sizeof(int) * result_width);
    char_matrix * result_matrix = (char_matrix*) malloc(sizeof(char_matrix));
    result_matrix->matrix = result_int_matrix;
    result_matrix->height = result_height;
    result_matrix->width = result_width;

    for(int i = 0; i < result_height; i++) {
        for(int j = 0; j < result_width; j++) {
            int max_conv = 0;
            int max_conv_ch = ' ';
            image_matrix m;
            init_image_matrix(&m, height, width);
            matrix_cpy(&m, image, j * width / (scale * 1.2), i * height / (scale * 1.2), height, width);
            for(int k = 0; k < count; k++) {
                int conv = conv_int_matrices((ascii_matrices[k]).matrix.matrix, m.matrix, m.height, m.width);
                if(conv > max_conv && (ascii_matrices[k]).ch != 'b' && (ascii_matrices[k]).ch != 'b' && (ascii_matrices[k]).ch != '$' && (ascii_matrices[k]).ch != 'X' && (ascii_matrices[k]).ch != 'e' && (ascii_matrices[k]).ch != 'o' && (ascii_matrices[k]).ch != '8' && (ascii_matrices[k]).ch != 'B' && (ascii_matrices[k]).ch != 'k' && (ascii_matrices[k]).ch != 'V' && (ascii_matrices[k]).ch != 'a' && (ascii_matrices[k]).ch != 'R' && (ascii_matrices[k]).ch != '0' && (ascii_matrices[k]).ch != 'd' && (ascii_matrices[k]).ch != 'N'  && (ascii_matrices[k]).ch != 'M'  && (ascii_matrices[k]).ch != 'B'  && (ascii_matrices[k]).ch != 'g'  && (ascii_matrices[k]).ch != 'O'  && (ascii_matrices[k]).ch != '@'  && (ascii_matrices[k]).ch != 'W' && (ascii_matrices[k]).ch != '#' && (ascii_matrices[k]).ch != '%' && (ascii_matrices[k]).ch != 'm' && (ascii_matrices[k]).ch != 'Q') {
                    max_conv = conv;
                    max_conv_ch = (ascii_matrices[k]).ch;
                }
            }
            result_int_matrix[i][j] = max_conv_ch;
        }
    }
    return result_matrix;
}