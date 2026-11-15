# Makefile for lab02 (GNU Make, Linux/macOS/WSL)
# Windows/MinGW: use mingw32-make and set TARGET = vpd_lab2.exe

CC     = gcc
OPT   ?= -O0
CFLAGS = -Wall -Wextra -Wpedantic $(OPT)
TARGET = vpd_lab2
OBJ    = main.o device.o timer.o

# Link: $@ = target (vpd_lab2), $^ = all prerequisites (main.o device.o timer.o)
$(TARGET): $(OBJ)
	$(CC) $^ -o $@

# Compile any .c into .o: $< = first prerequisite (the .c file), $@ = the .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Header dependencies (rebuild when a header changes)
main.o:   main.c device.h timer.h
device.o: device.c device.h
timer.o:  timer.c timer.h

.PHONY: run debug release compare stages clean

run: $(TARGET)
	./$(TARGET)

debug: clean
	$(MAKE) OPT="-O0 -g"

release: clean
	$(MAKE) OPT="-O2"

# Build -O0 and -O2 side by side, show file sizes and run both
compare:
	$(CC) -Wall -Wextra -Wpedantic -O0 main.c device.c timer.c -o vpd_lab2_O0
	$(CC) -Wall -Wextra -Wpedantic -O2 main.c device.c timer.c -o vpd_lab2_O2
	ls -l vpd_lab2_O0 vpd_lab2_O2
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
	gcc main.o device.o timer.o -o $(TARGET)

clean:
	rm -f *.o *.i *.s $(TARGET) vpd_lab2_O0 vpd_lab2_O2
