CC = gcc
CFLAGS = -O3
LIBS = -fopenmp

DEFINES = PROD

unisex: unisex.c
	$(CC) $(CFLAGS) -D$(DEFINES) -o $@.o $@.c $(LIBS)
