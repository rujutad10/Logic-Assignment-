CC = gcc
CFLAGS = -Wall -g -std=c11
SRC = main.c common.c cnf_reader.c task1.c task2.c task3.c task4.c task5.c task6.c task7.c
BIN = logic

all:
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

clean:
	rm -f $(BIN) *.o
