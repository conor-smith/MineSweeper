CC=gcc
CFLAGS=-g -Wall
SRC=src
BIN=bin

$(BIN)/textui : $(SRC)/textui.c $(SRC)/minesweeper.c
	$(CC) $(CFLAGS) -o $@ $^

test: $(BIN)/minesweepertest
	./$^

$(BIN)/minesweepertest: libraries/unity.c $(SRC)/minesweepertest.c $(SRC)/minesweeper.c
	$(CC) $(CFLAGS) -I libraries -o $@ $^

clean:
	rm $(OBJ)/* $(BIN)/*