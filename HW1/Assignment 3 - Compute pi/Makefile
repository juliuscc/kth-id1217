CC = gcc
LIBS = -lpthread -lm
CFLAGS = -O3

TARGETS = compute_pi

compute_pi: compute_pi.c
	$(CC) $(CFLAGS) -o $@.o $@.c $(LIBS)

clean:
	rm -f *.o *.exe $(TARGETS)