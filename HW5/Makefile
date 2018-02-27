CC = mpicc
LIBS = 
CFLAGS = # -O3

EXEC = mpiexec

SOURCE = src
BUILD = build
RESULT = result

TARGETS = centralized symmetric ring
DEFINES = NONE
# DEFINES = NICEPRINT

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

# Benchmark code
benchmark-centralized: centralized
	@mkdir -p $(RESULT)
	@rm -f $(RESULT)/$<-benchmark-result.md
	@echo "|Procecess | 1 run | 2 runs | 3 runs|" >> $(RESULT)/$<-benchmark-result.md
	@echo "|--|--|--|" >> $(RESULT)/$<-benchmark-result.md
	# 2 p
	@printf "| 2 | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 2 $(BUILD)/$< 1 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 2 $(BUILD)/$< 2 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 2 $(BUILD)/$< 3 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	# 4 p
	@printf "\n| 4 | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 4 $(BUILD)/$< 1 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 4 $(BUILD)/$< 2 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 4 $(BUILD)/$< 3 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	# 6 p
	@printf "\n| 6 | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 6 $(BUILD)/$< 1 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 6 $(BUILD)/$< 2 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 6 $(BUILD)/$< 3 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	# 8 p
	@printf "\n| 8 | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 8 $(BUILD)/$< 1 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 8 $(BUILD)/$< 2 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 8 $(BUILD)/$< 3 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md


benchmark-symmetric: symmetric
	@mkdir -p $(RESULT)
	@rm -f $(RESULT)/$<-benchmark-result.md
	@echo "|Procecess | 1 run | 2 runs | 3 runs|" >> $(RESULT)/$<-benchmark-result.md
	@echo "|--|--|--|" >> $(RESULT)/$<-benchmark-result.md
	# 2 p
	@printf "| 2 | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 2 $(BUILD)/$< 1 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 2 $(BUILD)/$< 2 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 2 $(BUILD)/$< 3 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	# 4 p
	@printf "\n| 4 | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 4 $(BUILD)/$< 1 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 4 $(BUILD)/$< 2 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 4 $(BUILD)/$< 3 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	# 6 p
	@printf "\n| 6 | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 6 $(BUILD)/$< 1 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 6 $(BUILD)/$< 2 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 6 $(BUILD)/$< 3 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	# 8 p
	@printf "\n| 8 | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 8 $(BUILD)/$< 1 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 8 $(BUILD)/$< 2 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 8 $(BUILD)/$< 3 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md

benchmark-ring: ring
	@mkdir -p $(RESULT)
	@rm -f $(RESULT)/$<-benchmark-result.md
	@echo "|Procecess | 1 run | 2 runs | 3 runs|" >> $(RESULT)/$<-benchmark-result.md
	@echo "|--|--|--|" >> $(RESULT)/$<-benchmark-result.md
	# 2 p
	@printf "| 2 | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 2 $(BUILD)/$< 1 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 2 $(BUILD)/$< 2 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 2 $(BUILD)/$< 3 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	# 4 p
	@printf "\n| 4 | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 4 $(BUILD)/$< 1 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 4 $(BUILD)/$< 2 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 4 $(BUILD)/$< 3 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	# 6 p
	@printf "\n| 6 | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 6 $(BUILD)/$< 1 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 6 $(BUILD)/$< 2 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 6 $(BUILD)/$< 3 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	# 8 p
	@printf "\n| 8 | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 8 $(BUILD)/$< 1 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 8 $(BUILD)/$< 2 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md
	mpiexec -np 8 $(BUILD)/$< 3 >> $(RESULT)/$<-benchmark-result.md
	@printf " | " >> $(RESULT)/$<-benchmark-result.md

# Clean code
clean:
	rm -f *.o *.exe $(TARGETS)