CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic
SRC = png_to_int.c matrix_processing.c test.c
OBJ = png_to_int.o test.o

all: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o test -lpng -lm && ./test

clean:
	rm -f *.o test