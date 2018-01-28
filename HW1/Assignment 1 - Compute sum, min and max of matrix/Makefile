CC = gcc
LIBS = -lpthread -lm
CFLAGS = -O3

TARGETS = matrixSum matrixSum.a matrixSum.b matrixSum.c

matrixSum: matrixSum.c
	$(CC) $(CFLAGS) -o $@ $@.c $(LIBS)

matrixSum.a: matrixSum.a.c
	$(CC) $(CFLAGS) -o $@ $@.c $(LIBS)

matrixSum.b: matrixSum.b.c
	$(CC) $(CFLAGS) -o $@ $@.c $(LIBS)

matrixSum.c: matrixSum.c.c
	$(CC) $(CFLAGS) -o $@ $@.c $(LIBS)

clean:
	rm -f *.o *.exe $(TARGETS)