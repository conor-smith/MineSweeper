#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "enums.h"
#include "minesweeper.h"

typedef struct MineSweeperGame {
    int length, height, mines, flagged, revealed;
    tile** board;
    state gameState;
    bool gameStarted;
} Game;

// This is only for internal use. Exists to make things easier
typedef struct {
    int x, y;
}  Position;

// These are 'private' functions. They are for internal use and are not declared in the header file

/* Returns double pointer of tiles
 * Creates an array of size length * height, then assigns it to a pointer
 * Assigns subsequent pointers to multiples of length, creating a 2D array*/
tile** createBoard(int length, int height) {
    tile** board = malloc(sizeof(tile*) * length);
    tile* tileArray = calloc(length * height, sizeof(tile));

    for(int i = 0;i < length;i++) {
        board[i] = tileArray + (height * i);
    }

    return board;
}

// Frees the board specifically
void freeBoard(tile** board) {
    free(board[0]);
    free(board);
}

// Simply checks to see if move is legal
bool moveIsWithinBoundaries(Game* game, int x, int y) {
    return x >= 0 && y >= 0 && x < game->length && y < game->height;
}

// Takes an empty game object, and sets up internals
void initializeGame(Game* game, int length, int height, int mines) {
    length = length >= 1 ? length : 1;
    height = height >= 1 ? height : 1;
    mines = mines >= 0 ? mines : 0;
    mines = mines <= length * height ? mines : length * height;

    game->length = length;
    game->height = height;
    game->mines = mines;
    game->flagged = 0;
    game->revealed = 0;
    game->gameState = ACTIVE;
    game->gameStarted = false;
    game->board = createBoard(length, height);
}

/* Gets co-ordinates surrouning a location, and prints data to data pointer
 * data pointer will be a pointer to an array of positions defined on the stack by the calling function
 * The very first position.x will contain the array size including the first position*/
void getSurroundingPositions(Position* data, Game* game, int x, int y) {
    int counter = 1;
    for(int xPosition = x-1;xPosition <= x+1;xPosition++) {
        for(int yPosition = y-1;yPosition <= y+1;yPosition++) {
            if(xPosition >= 0 && yPosition >= 0 && xPosition < game->length && yPosition < game->height && !(xPosition == x && yPosition == y)) {
                data[counter].x = xPosition;
                data[counter].y = yPosition;
                counter++;
            }
        }
    }

    data[0].x = counter;
}

/* An implementation of the fisher yates shuffle
 * Shuffles elements in data array inbetween start and end*/
void shuffle(Position* data, int start, int end) {

    srand(time(0));

    for(int i = end - 1;i > start;i--) {
        int indexToSwap = rand() % (end - start) + start;
        Position swap = data[i];
        data[i] = data[indexToSwap];
        data[indexToSwap] = swap;
    }
}

/* This exists only for placeMines
 * It simply checks if a position already exists in an array*/
bool contains(Position* positions, int x, int y) {
    for(int i = 1;i < positions[0].x;i++) {
        if(positions[i].x == x && positions[i].y == y) {
            return true;
        }
    }

    return false;
}

/* This essentially places all tiles in a 1Dimensional array, shuffles them, then assigns mines sequentially until none remain
 * This only occurs after the first move. The board is essentially empty before this
 * The selected tile (x, y) is placed at the end of the array, surrounding tiles before, then all other tiles first
 * Each of these "sections" are shuffled individually. They are marked by surroundingStart and surroundingEnd
 * This ensures that the selected tile, and those immediately surrounding, are the last to be made mines*/
void placeMines(Game* game, int x, int y) {
    // These are used later when shuffling the array
    int surroundingStart, surroundingEnd;

    // Every tile is represented by a position in this array
    Position* positionList = malloc(sizeof(Position) * game->length * game->height);

    // Gets positions surrounding position
    Position surrounding[9];
    getSurroundingPositions(surrounding, game, x, y);

    // Initializes list of positions
    surroundingEnd = 0;
    for(int xValue = 0;xValue < game->length;xValue++) {
        for(int yValue = 0;yValue < game->height;yValue++) {
            //Checks to see if position is in surrounding
            if(!contains(surrounding, xValue, yValue) && !(xValue == x && yValue == y)) {
                positionList[surroundingEnd].x = xValue;
                positionList[surroundingEnd].y = yValue;
                surroundingEnd++;
            }
        }
    }

    surroundingStart = surroundingEnd;

    for(int i = 1;i < surrounding[0].x;i++) {
        positionList[surroundingEnd] = surrounding[i];
        surroundingEnd++;
    }

    positionList[surroundingEnd].x = x;
    positionList[surroundingEnd].y = y;

    // Shuffles sections of array
    shuffle(positionList, 0, surroundingStart);
    shuffle(positionList, surroundingStart, surroundingEnd);

    // Converts tiles to mines
    for(int i = 0;i < game->mines;i++) {
        game->board[positionList[i].x][positionList[i].y] += 9;
    }

    // Counts surrounding mines for remaining tiles
    for(int i = game->mines;i < game->length * game->height;i++) {
        Position surrounding[9];
        getSurroundingPositions(surrounding, game, positionList[i].x, positionList[i].y);

        for(int j = 1;j < surrounding[0].x;j++) {
            game->board[positionList[i].x][positionList[i].y] += (game->board[surrounding[j].x][surrounding[j].y] == HIDDEN_MINE || game->board[surrounding[j].x][surrounding[j].y] == FLAGGED_MINE);
        }
    }

    // Free positionList
    free(positionList);
}

/* Recursively reveals tiles
 * If a tile has 0 surrounding mines, it will reveal surrounding tiles as well
 * It is assumed that there will never be an illegal x or y value input*/
void revealAll(Game* game, int x, int y) {
    if(game->board[x][y] < 10) {
        /* Reveal tile
         * As of this time, it is still unknown how many mines are surrounding it*/
        game->board[x][y] += 20;
        game->revealed++;

        // If no surrounding mines, reveal all surrounding tiles
        if(game->board[x][y] == REVEALED_0) {
            Position surrounding[9];
            getSurroundingPositions(surrounding, game, x, y);

            for(int i = 1;i < surrounding[0].x;i++) {
                revealAll(game, surrounding[i].x, surrounding[i].y);
            }
        }
    }
}

// End of private functions

// Creates a new game
Game* createGame(int length, int height, int mines) {
    Game* game = malloc(sizeof(Game));

    initializeGame(game, length, height, mines);

    return game;
}

// Resets internals of existing game
void resetGame(Game* game, int length, int height, int mines) {
    freeBoard(game->board);
    initializeGame(game, length, height, mines);
}

// Frees all memory associated with game
void deleteGame(Game* game) {
    freeBoard(game->board);
    free(game);
}

// Reveals a tile and all surrounding tiles (if empty)
state reveal(Game* game, int x, int y) {
    //Check if move is legal
    if(!moveIsWithinBoundaries(game, x, y) || game->gameState != ACTIVE) {
        return game->gameState;
    }

    //If this is the first move, mines must be placed
    if(!game->gameStarted) {
        placeMines(game, x, y);
    }

    if(game->board[x][y] == HIDDEN_MINE) {
        game->board[x][y] += 20;
        game->revealed++;
        game->gameState = LOSS;
        return game->gameState;
    } else {
        revealAll(game, x, y);
    }

    game->gameState = game->gameState + (game->revealed == game->length * game->height - game->mines);

    return game->gameState;
}

/* Flags a hidden tile
 * Unflags a flagged tile
 * Does nothing to revealed tile*/
void flag(Game* game, int x, int y) {
    if(moveIsWithinBoundaries(game, x, y) && game->gameState == ACTIVE) {
        // Optimize later
        // 0=hidden, 1= flagged, 2=revealed
        int tileState = game->board[x][y] / 10;

        switch(tileState) {
            case 1:
                game->board[x][y] -= 10;
                game->flagged--;
                break;
            case 0:
                game->board[x][y] += 10;
                game->flagged++;
                break;
        }
    }
}

int getLength(Game* game) {
    return game->length;
}
int getHeight(Game* game) {
    return game->height;
}
int getMines(Game* game){
    return game->mines;
}
int getFlagged(Game* game) {
    return game->flagged;
}
int getRevealed(Game* game) {
    return game->revealed;
}
bool getGameStarted(Game* game) {
    return game->gameStarted;
}
state getGameState(Game* game) {
    return game->gameState;
}
tile** getBoard(Game* game) {
    return game->board;
}