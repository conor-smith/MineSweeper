CC=gcc
CFLAGS=-g -Wall
SRC=src
OBJ=obj
BIN=bin

$(BIN)/textui : $(SRC)/textui.c $(SRC)/minesweeper.c
	$(CC) $(CFLAGS) -o $@ $^

test: $(BIN)/minesweepertest
	./$^

$(BIN)/minesweepertest: $(OBJ)/unity.o $(SRC)/minesweepertest.c
	$(CC) $(CFLAGS) -I libraries -o $@ $^

$(OBJ)/unity.o: libraries/unity.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm $(OBJ)/* $(BIN)/*