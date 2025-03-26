#include "pti.h"
#define BINFILEPATH "bin/ascii"
#define FILEPATH "ascii_7_step.png"
#define ASCII_SYM_WIDTH 7
#define ASCII_SYM_HEIGHT 16
#define ASCII_COUNT 95
void simple_print_image(image_matrix * matrix) {
    printf("size is: %d x %d\n", matrix->height, matrix->width);
    for (int y = 0; y < matrix->height; y++) {
        for (int x = 0; x < 64; x++) {
            printf("%c", (matrix->matrix)[y][x] > 150 ? '#' : ' ');
        }
        printf("$\n");
    }
}

void matrix_cpy(image_matrix * m1, image_matrix * m2, int index_x, int index_y, int height, int width) {
    for(int i = index_y; i < height; i ++) {
        for(int j = index_x; j < width; j ++) {
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

void read_ascii_matrix_from_bin(ascii_matrix * ascii_matrix, FILE * fp) {
    fread(ascii_matrix, sizeof(ascii_matrix), 1, fp);
}

int main() {
    image_matrix ascii_image_matrix;
    read_png_file(FILEPATH, &ascii_image_matrix);
    
    ascii_matrix * ascii = create_ascii_matrices(&ascii_image_matrix);

    destroy_image_matrix(&ascii_image_matrix);
    return 0;
}