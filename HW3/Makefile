CC = gcc
CFLAGS = -O3
LIBS = -fopenmp

DEFINES = PROD

unisex: unisex.c
	$(CC) $(CFLAGS) -D$(DEFINES) -o $@.o $@.c $(LIBS)

# benchmark-matrixSum-float: matrixSum.c.float
# 	@mkdir -p $(RESULT)
# 	$(BUILD)/$<.o > $(RESULT)/$@.md