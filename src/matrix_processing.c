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

void linear_contrast(image_matrix * m) {
    int min = 255, max = 0;

    // Находим min и max
    for (int y = 0; y < m->height; y++) {
        for (int x = 0; x < m->width; x++) {
            if ((m->matrix)[y][x] < min) min = (m->matrix)[y][x];
            if ((m->matrix)[y][x] > max) max = (m->matrix)[y][x];
        }
    }

    // Линейное преобразование
    for (int y = 0; y < m->height; y++) {
        for (int x = 0; x < m->width; x++) {
            if (max != min) {  // Избегаем деления на 0
                (m->matrix)[y][x] = ((m->matrix)[y][x] - min) * 255 / (max - min);
            }
        }
    }
}

void gamma_correction(image_matrix * m, double gamma) {
    for (int y = 0; y < m->height; y++) {
        for (int x = 0; x < m->width; x++) {
            double normalized = (double)(m->matrix)[y][x] / 255.0;
            double corrected = pow(normalized, 1.0 / gamma);
            (m->matrix)[y][x] = (int)(corrected * 255);
        }
    }
}

void thresholding(image_matrix * m, int threshold) {
    for (int y = 0; y < m->height; y++) {
        for (int x = 0; x < m->width; x++) {
            (m->matrix)[y][x] = ((m->matrix)[y][x] > threshold) ? 255 : 0;
        }
    }
}

void histogram_equalization(image_matrix * m) {
    int hist[256] = {0};
    int cumul_hist[256] = {0};

    // Строим гистограмму
    for (int y = 0; y < m->height; y++) {
        for (int x = 0; x < m->width; x++) {
            hist[(m->matrix)[y][x]]++;
        }
    }

    // Кумулятивная гистограмма
    cumul_hist[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cumul_hist[i] = cumul_hist[i - 1] + hist[i];
    }

    // Нормализация и преобразование
    int total_pixels = m->height * m->width;
    for (int y = 0; y < m->height; y++) {
        for (int x = 0; x < m->width; x++) {
            (m->matrix)[y][x] = 255 * cumul_hist[(m->matrix)[y][x]] / total_pixels;
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

char_matrix * conv_ascii_matrix(image_matrix * image, ascii_matrix * ascii_matrices, int count, int height, int width, int scale, char * blacklist) {
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
                char * in_blacklist = strchr(blacklist, (ascii_matrices[k]).ch);
                if(conv > max_conv && in_blacklist == NULL) {
                    max_conv = conv;
                    max_conv_ch = (ascii_matrices[k]).ch;
                }
            }
            result_int_matrix[i][j] = max_conv_ch;
        }
    }
    return result_matrix;
}