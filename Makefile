# Lab 2 Makefile (GNU Make). On Windows/MinGW: use mingw32-make, TARGET = vpd_lab2.exe
CC      = gcc
OPT    ?= -O0
CFLAGS  = -Wall -Wextra -Wpedantic $(OPT)
TARGET  = vpd_lab2
OBJ     = main.o device.o timer.o

# --- automatic variables version: $@ = target, $< = first prerequisite, $^ = all prerequisites
$(TARGET): $(OBJ)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

main.o:   main.c device.h timer.h
device.o: device.c device.h
timer.o:  timer.c timer.h

.PHONY: run debug release compare stages exp std clean

run: $(TARGET)
	./$(TARGET)

debug:
	$(MAKE) clean
	$(MAKE) OPT="-O0 -g" TARGET=vpd_lab2

release:
	$(MAKE) clean
	$(MAKE) OPT="-O2" TARGET=vpd_lab2

# Build -O0 and -O2 binaries side by side and run both
compare:
	$(CC) -Wall -Wextra -Wpedantic -O0 main.c device.c timer.c -o vpd_lab2_O0
	$(CC) -Wall -Wextra -Wpedantic -O2 main.c device.c timer.c -o vpd_lab2_O2
	@ls -l vpd_lab2_O0 vpd_lab2_O2
	@echo "===== -O0 =====" && ./vpd_lab2_O0
	@echo "===== -O2 =====" && ./vpd_lab2_O2

# Manual stages: .c -> .i -> .s -> .o -> executable
stages:
	gcc -E main.c -o main.i
	gcc -E -P main.c -o main_no_lines.i
	gcc -S main.c -o main.s
	gcc -c main.c -o main.o
	gcc -c device.c -o device.o
	gcc -c timer.c -o timer.o
	gcc main.o device.o timer.o -o vpd_lab2

# Small experiments from experiments/ (one binary per file)
EXP_SRC = types arrays chars printf_demo scanf_demo operators register macros
exp:
	@mkdir -p experiments/bin
	@for f in $(EXP_SRC); do \
	  echo "gcc $$f.c"; \
	  gcc -Wall -Wextra -Wpedantic experiments/$$f.c -o experiments/bin/$$f || exit 1; \
	done
	g++ -Wall -Wextra experiments/foreach.cpp -o experiments/bin/foreach

# C standard probe
std:
	@mkdir -p experiments/bin
	@for s in c90 c99 c11 c17 c23; do \
	  echo "== -std=$$s =="; \
	  gcc -std=$$s -Wall -Wextra -Wpedantic experiments/standard_probe.c -o experiments/bin/probe_$$s && ./experiments/bin/probe_$$s; true; \
	done
	@echo "(more features: experiments/std_features.txt)"

clean:
	rm -f *.o *.i *.s $(TARGET) vpd_lab2_O0 vpd_lab2_O2 vpd_lab2.exe
	rm -rf experiments/bin
