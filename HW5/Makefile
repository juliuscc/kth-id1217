CC = gcc
LIBS = -lpthread -lm
CFLAGS = -O3

SOURCE = src
BUILD = build
RESULT = result

TARGETS = centralized symmetric ring
DEFINES = NONE

all: $(TARGETS)
.PHONY: all

centralized: $(SOURCE)/centralized.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -D $(DEFINES) -o $(BUILD)/$@.o $(SOURCE)/$@.c $(LIBS)

symmetric: $(SOURCE)/symmetric.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -D $(DEFINES) -o $(BUILD)/$@.o $(SOURCE)/$@.c $(LIBS)

ring: $(SOURCE)/ring.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -D $(DEFINES) -o $(BUILD)/$@.o $(SOURCE)/$@.c $(LIBS)

# benchmark: compute_pi
# 	rm -rf results \
# 	&& mkdir results \
# 	&& ./benchmark.sh ./compute_pi.o results/compute_pi.results
	
clean:
	rm -f *.o *.exe $(TARGETS)