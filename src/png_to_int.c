#include "pti.h"

void init_image_matrix(image_matrix * matrix, int height, int width) {
    matrix->width = width;
    matrix->height = height;
    matrix->matrix = (int **) malloc(sizeof(int *) * height);
    for (int y = 0; y < height; y++) {
        (matrix->matrix)[y] = (int *) malloc(sizeof(int) * width);
    }
}

void destroy_image_matrix(image_matrix * matrix) {
    for (int y = 0; y < matrix->height; y++) {
        free((matrix->matrix)[y]);
    }
    free(matrix->matrix);
}

void read_png_file(const char* filename, image_matrix * matrix) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Ошибка: не удалось открыть файл %s\n", filename);
        exit(1);
    }

    // Проверка сигнатуры PNG
    png_byte header[8];
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)) {
        fprintf(stderr, "Ошибка: файл %s не является PNG\n", filename);
        fclose(fp);
        exit(1);
    }

    // Инициализация структур libpng
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fprintf(stderr, "Ошибка: png_create_read_struct\n");
        fclose(fp);
        exit(1);
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fprintf(stderr, "Ошибка: png_create_info_struct\n");
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        exit(1);
    }

    // Настройка обработки ошибок
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Ошибка во время чтения PNG\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        exit(1);
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    // Проверка размеров изображения
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    // Преобразование в 8-битный grayscale (если нужно)
    // png_set_strip_16(png_ptr);  // Если PNG 16-битный — преобразуем в 8-битный
    // png_set_gray_to_rgb(png_ptr);  // Если нужно RGB
    png_set_strip_16(png_ptr);
    png_set_strip_alpha(png_ptr);
    png_set_rgb_to_gray(png_ptr, PNG_ERROR_ACTION_NONE, -1, -1);
    // Чтение изображения
    png_bytep *row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
    for (int y = 0; (unsigned) y < height; y++) {
        row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));
    }
    init_image_matrix(matrix, height, width);
    png_read_image(png_ptr, row_pointers);
    
    // Заполнение матрицы (здесь можно выбрать канал R, G, B или яркость)
    for (int y = 0; (unsigned) y < height; y++) {
        png_byte* row = row_pointers[y];
        for (int x = 0; (unsigned) x < width; x++) {
            (matrix->matrix)[y][x] = row[x];
        }
        free(row_pointers[y]);
    }
    free(row_pointers);

    png_read_end(png_ptr, NULL);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
}