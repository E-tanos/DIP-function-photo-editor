# Compiler
CC = gcc
CFLAGS = -Wall -g

# Source files
SRCS = PhotoLab.c Image.c DIPs.c Advanced.c FileIO.c Test.c
OBJS = $(SRCS:.c=.o)

# Executable names
EXEC1 = PhotoLab
EXEC2 = PhotoLabTest

# Default target: build both executables
all: $(EXEC1) $(EXEC2) setup

# Setup: Copy necessary files
setup:
	cp ~eecs22/public/EngPlaza.ppm .
	cp ~eecs22/public/watermark_template.ppm .
	cp ~eecs22/public/border.ppm .

# Compile PhotoLab
$(EXEC1): PhotoLab.o Image.o DIPs.o Advanced.o FileIO.o Test.o
	$(CC) $(CFLAGS) -o $(EXEC1) PhotoLab.o Image.o DIPs.o Advanced.o FileIO.o Test.o

# Compile PhotoLabTest (ensure main() is included)
$(EXEC2): PhotoLab.o Test.o Image.o DIPs.o Advanced.o FileIO.o
	$(CC) $(CFLAGS) -o $(EXEC2) PhotoLab.o Test.o Image.o DIPs.o Advanced.o FileIO.o

# Run PhotoLabTest under Valgrind
valgrind: $(EXEC2)
	valgrind --leak-check=full ./$(EXEC2)

# Compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean all generated files
clean:
	rm -f $(OBJS) $(EXEC1) $(EXEC2) *.ppm *.jpg PhotoLab.script
