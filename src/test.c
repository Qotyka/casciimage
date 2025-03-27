#include "pti.h"

void simple_print_image(image_matrix * matrix) {
    printf("size is: %d x %d\n", matrix->height, matrix->width);
    for (int y = 0; y < matrix->height; y++) {
        for (int x = 0; x < 64; x++) {
            printf("%c", (matrix->matrix)[y][x] > 100 ? 'Q' : ' ');
        }
        printf("endline\n");
    }
}

void print_ascii_image(char_matrix * matrix) {
    printf("size is: %d x %d\n", matrix->height, matrix->width);
    for (int y = 0; y < matrix->height; y++) {
        for (int x = 0; x < matrix->width; x++) {
            printf("%c", (matrix->matrix)[y][x]);
        }
        printf("\n");
    }
}

#define BLACKLIST "@%Q#WgBmON0RMdXkabB8&VpZKD26EUHeho$A"

int main() {
    char_matrix * resulting_ascii_art;
    image_matrix ascii_full_matrix;
    image_matrix image;
    int scale = 1;
    read_png_file(FILEPATH, &ascii_full_matrix);
    read_png_file("../test/images/test_image_10.png", &image);
    gamma_correction(&image, 0.45);
    linear_contrast(&image);
    // histogram_equalization(&image);
    // thresholding(&image, 70);

    ascii_matrix * ascii_matrices = create_ascii_matrices(&ascii_full_matrix);
    resulting_ascii_art = conv_ascii_matrix(&image, ascii_matrices, ASCII_COUNT, ASCII_SYM_HEIGHT, ASCII_SYM_WIDTH, scale, BLACKLIST);
    print_ascii_image(resulting_ascii_art);
    return 0;
}