CC = gcc
CFLAGS = -Wall -g -std=c11

# 1. Removed 'common.c' (it's unused)
# 2. Fixed typo 'cnf_reader.c' to 'cnfReader.c'
# 3. WARNING: Your file list shows 'mainhc.c' but this Makefile uses 'main.c'
#    Make sure your main file is actually named 'main.c'
SRC = main.c cnfReader.c task1.c task2.c task3.c task4.c task5.c task6.c task7.c

# Added LDFLAGS for the math library (-lm)
LDFLAGS = -lm

BIN = logic

all:
	# Added $(LDFLAGS) to the linking command
	$(CC) $(CFLAGS) $(SRC) -o $(BIN) $(LDFLAGS)

clean:
	rm -f $(BIN) *.o
