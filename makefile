CC=gcc
CFLAGS=-g -Wall
LIB=libraries
SRC=src
TEST=test
BIN=bin

$(BIN)/textui : $(SRC)/textui.c $(SRC)/minesweeper.c
	$(CC) $(CFLAGS) -o $@ $^

test: $(BIN)/minesweepertest
	./$^

$(BIN)/minesweepertest: $(LIB)/unity.c $(TEST)/minesweepertest.c $(SRC)/minesweeper.c
	$(CC) $(CFLAGS) -I $(LIB) -I $(SRC) -o $@ $^

clean:
	rm $(BIN)/*