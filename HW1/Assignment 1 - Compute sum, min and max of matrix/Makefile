CC = gcc
LIBS = -lpthread -lm
CFLAGS = -O3

TARGETS = matrixSum.original matrixSum.a matrixSum.b matrixSum.c

all : $(TARGETS)
.PHONY : all

matrixSum.original: matrixSum.original.c
	$(CC) $(CFLAGS) -o $@.o $@.c $(LIBS)

matrixSum.a: matrixSum.a.c
	$(CC) $(CFLAGS) -o $@.o $@.c $(LIBS)

matrixSum.b: matrixSum.b.c
	$(CC) $(CFLAGS) -o $@.o $@.c $(LIBS)

matrixSum.c: matrixSum.c.c
	$(CC) $(CFLAGS) -o $@.o $@.c $(LIBS)

clean:
	rm -f *.o *.exe $(TARGETS)