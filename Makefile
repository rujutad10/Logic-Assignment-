CC = gcc
CFLAGS = -Wall -g -std=c11
LDFLAGS = -lm

# 1. Removed 'common.c', fixed 'cnfReader.c'
# 2. WARNING: Make sure your main file is 'main.c' (not 'mainhc.c')
SRC = main.c cnfReader.c task1.c task2.c task3.c task4.c task5.c task6.c task7.c

# Automatically create a list of object files (e.g., main.o, task1.o)
OBJ = $(SRC:.c=.o)

BIN = logic

# The 'all' rule now depends on the final binary
all: $(BIN)

# The binary depends on all the object files
$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN) $(LDFLAGS)

# This is a pattern rule. It tells 'make' how to build
# any .o file from its corresponding .c file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule now also removes the object files
clean:
	rm -f $(BIN) $(OBJ)
