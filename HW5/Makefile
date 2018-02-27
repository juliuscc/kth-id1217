CC = mpicc
LIBS = 
CFLAGS = # -O3

EXEC = mpiexec

SOURCE = src
BUILD = build
RESULT = result

TARGETS = centralized symmetric ring
DEFINES = NONE

all: $(TARGETS)
.PHONY: all

# Build code
centralized: $(SOURCE)/centralized.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -D$(DEFINES) -o $(BUILD)/$@ $(SOURCE)/$@.c $(LIBS)

symmetric: $(SOURCE)/symmetric.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -D$(DEFINES) -o $(BUILD)/$@ $(SOURCE)/$@.c $(LIBS)

ring: $(SOURCE)/ring.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -D$(DEFINES) -o $(BUILD)/$@ $(SOURCE)/$@.c $(LIBS)

# Run code
run-centralized: centralized
	mpiexec -np 4 $(BUILD)/centralized

run-symmetric: symmetric
	mpiexec -np 4 $(BUILD)/symmetric

run-ring: ring
	mpiexec -np 4 $(BUILD)/ring

# Clean code
clean:
	rm -f *.o *.exe $(TARGETS)