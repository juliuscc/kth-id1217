CC = gcc
LIBS = -lpthread -lm
CFLAGS = -O3

TARGETS = compute_pi
DEFINES = STRIPPEDPRINT

compute_pi: compute_pi.c
	$(CC) $(CFLAGS) -D $(DEFINES) -o $@.o $@.c $(LIBS)

benchmark: compute_pi
	rm -rf results \
	&& mkdir results \
	&& ./benchmark.sh ./compute_pi.o results/compute_pi.results
	
clean:
	rm -f *.o *.exe $(TARGETS)