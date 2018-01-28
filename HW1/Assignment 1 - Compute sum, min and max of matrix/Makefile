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

# Need to have hyperfine installed
benchmark_original: matrixSum.original
	hyperfine './$<.o 10000 1' \
	'./$<.o 10000 2' \
	'./$<.o 10000 4' \
	'./$<.o 10000 8'

# Need to have hyperfine installed
benchmark_a: matrixSum.a
	hyperfine './$<.o 10000 1' \
	'./$<.o 10000 2' \
	'./$<.o 10000 4' \
	'./$<.o 10000 8' 

# Need to have hyperfine installed
benchmark_b: matrixSum.b
	hyperfine './$<.o 10000 1' \
	'./$<.o 10000 2' \
	'./$<.o 10000 4' \
	'./$<.o 10000 8'
# Need to have hyperfine installed
benchmark_c: matrixSum.c
	hyperfine './$<.o 10000 1' \
	'./$<.o 10000 2' \
	'./$<.o 10000 4' \
	'./$<.o 10000 8'

	rm -f *.o *.exe $(TARGETS)