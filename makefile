CC = gcc
PROG = MineSweeper
DEBUGFLAGS = -g
BIN = bin
SRC = src

all: $(PROG)

$(PROG): $(BIN)/minesweeper.o $(BIN)/init.o $(BIN)/main.o $(BIN)/input.o $(BIN)/draw.o $(BIN)/window.o
	$(CC) $(DEBUGFLAGS) -o $@ $^ `sdl2-config --libs` -lSDL2_mixer -lSDL2_image -lSDL2_ttf -lm

$(BIN)/init.o: $(SRC)/init.c
	@mkdir -p $(BIN)
	$(CC) $(DEBUGFLAGS) -o $@ $^ -c `sdl2-config --cflags`

$(BIN)/main.o: $(SRC)/main.c
	@mkdir -p $(BIN)
	$(CC) $(DEBUGFLAGS) -o $@ $^ -c `sdl2-config --cflags`

$(BIN)/minesweeper.o: $(SRC)/minesweeper.c
	@mkdir -p $(BIN)
	$(CC) $(DEBUGFLAGS) -o $@ $^  -c

$(BIN)/input.o: $(SRC)/input.c
	@mkdir -p $(BIN)
	$(CC) $(DEBUGFLAGS) -o $@ $^ -c `sdl2-config --cflags`

$(BIN)/draw.o: $(SRC)/draw.c
	@mkdir -p $(BIN)
	$(CC) $(DEBUGFLAGS) -o $@ $^ -c `sdl2-config --cflags`

$(BIN)/textui.o: $(SRC)/textui.c
	@mkdir -p $(BIN)
	$(CC) $(DEBUGFLAGS) -o $@ $^ -c

$(BIN)/window.o: $(SRC)/window.c
	@mkdir -p $(BIN)
	$(CC) $(DEBUGFLAGS) -o $@ $^ -c `sdl2-config --cflags`

textui: $(BIN)/minesweeper.o $(BIN)/textui.o
	$(CC) $(DEBUGFLAGS) -o $@ $^  -lncurses

clean:
	rm -rf $(BIN) $(PROG) textui