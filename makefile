CC=gcc
CFLAGS=-g -Wall
LIB=libraries
SRC=src
BIN=bin

$(BIN)/textui : $(SRC)/textui.c $(SRC)/minesweeper.c
	$(CC) $(CFLAGS) -o $@ $^

test: $(BIN)/minesweepertest
	./$^

$(BIN)/minesweepertest: $(LIB)/unity.c $(SRC)/minesweepertest.c $(SRC)/minesweeper.c
	$(CC) $(CFLAGS) -I $(LIB) -o $@ $^

clean:
	rm $(BIN)/*