CC = gcc
LIBS = -lpthread -lm
CFLAGS = -O3

TARGETS = matrixSum.original matrixSum.a matrixSum.b matrixSum.c

DEFINES = STRIPPEDPRINT

all : $(TARGETS)
.PHONY : all

matrixSum.original: matrixSum.original.c
	$(CC) $(CFLAGS) -D $(DEFINES) -o $@.o $@.c $(LIBS)

matrixSum.a: matrixSum.a.c
	$(CC) $(CFLAGS) -D $(DEFINES) -o $@.o $@.c $(LIBS)

matrixSum.b: matrixSum.b.c
	$(CC) $(CFLAGS) -D $(DEFINES) -o $@.o $@.c $(LIBS)

matrixSum.c: matrixSum.c.c
	$(CC) $(CFLAGS) -D $(DEFINES) -o $@.o $@.c $(LIBS)

benchmark: $(TARGETS)
	rm -rf results \
	&& mkdir results \
	&& ./benchmark.sh ./matrixSum.original.o results/original.results \
	&& ./benchmark.sh ./matrixSum.a.o results/a.results \
	&& ./benchmark.sh ./matrixSum.b.o results/b.results \
	&& ./benchmark.sh ./matrixSum.c.o results/c.results

clean:
	rm -f *.o *.exe $(TARGETS)