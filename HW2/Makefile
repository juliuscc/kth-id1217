CC = gcc
CFLAGS = -O3
LIBS = -fopenmp

SOURCE = src
BUILD = build
RESULT = result

DEFINES = PROD

TARGETS = matrixSum-openmp matrixSum.a matrixSum.b matrixSum.c matrixSum.c.float quicksort

all : $(TARGETS)
.PHONY : all

matrixSum-openmp: $(SOURCE)/matrixSum-openmp.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -D $(DEFINES) -o $(BUILD)/$@.o $(SOURCE)/$@.c $(LIBS)


matrixSum.a: $(SOURCE)/matrixSum.a.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -D $(DEFINES) -o $(BUILD)/$@.o $(SOURCE)/$@.c $(LIBS)


matrixSum.b: $(SOURCE)/matrixSum.b.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -D $(DEFINES) -o $(BUILD)/$@.o $(SOURCE)/$@.c $(LIBS)


matrixSum.c: $(SOURCE)/matrixSum.c.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -D $(DEFINES) -o $(BUILD)/$@.o $(SOURCE)/$@.c $(LIBS)

matrixSum.c.float: $(SOURCE)/matrixSum.c.float.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -D $(DEFINES) -o $(BUILD)/$@.o $(SOURCE)/$@.c $(LIBS)

quicksort: $(SOURCE)/quicksort.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -D $(DEFINES) -o $(BUILD)/$@.o $(SOURCE)/$@.c $(LIBS)

benchmark-matrixSum: matrixSum.c
	@mkdir -p $(RESULT)
	$(BUILD)/$<.o > $(RESULT)/$@.md

benchmark-matrixSum-float: matrixSum.c.float
	@mkdir -p $(RESULT)
	$(BUILD)/$<.o > $(RESULT)/$@.md

benchmark-quicksort: quicksort
	@mkdir -p $(RESULT)
	$(BUILD)/$<.o > $(RESULT)/$@.md