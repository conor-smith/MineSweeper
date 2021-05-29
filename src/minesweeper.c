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

typedef struct {
    int x, y;
}  Position;

//These are 'private' functions. They are for internal use and are not declared in the header file

/* Returns double pointer of tiles
 * Creates an array of size length * height, then assigns it to a pointer
 * Assigns subsequent pointers to multiples of length, creating a 2D array*/
tile** createBoard(int length, int height) {
    tile** board = malloc(sizeof(tile*) * length);
    tile* tileArray = calloc(length * height, sizeof(tile));

    for(int i = 0;i < length;i++) {
        board[i] = tileArray + length * i;
    }

    return board;
}

// Frees the board specificaly
void freeBoard(tile** board) {
    free(board[0]);
    free(board);
}

//Takes an empty game object, and sets up internals
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
 * data pointer will be a pointer to an array defined on the stack by the calling function*/
void getSurroundingSquares(int* data, int length, int height, Position position) {
    int* counter = data;
    for(int xValue = x-1;xValue <= x+1;xValue++) {
        for(int yValue = y;yValue <= y+1;yValue++) {
            if(xValue > 0 && yValue > 0 && xValue < length && yValue < height) {
                data[*counter * 2 + 1] = xValue;
                data[*counter * 2 + 2] = yValue;
                *counter++;
            }
        }
    }
}

/*

/* This essentially places all tiles in a 1Dimensional array, shuffles them, then assigns mines sequentially until none remain
 * This only occurs after the first. The board is essentially empty before this
 * The selected tile (x, y) is placed at the end of the array, surrounding tiles before, then all other tiles first
 * This ensures that the selected tile, and those immediately surrounding, are the last to be made mines*/
void placeMines(tile** board, int length, int height, Position position) {

    //Every tile is represented by a position in this array
    Position* positionList = malloc(sizeof(Position) * length * height);

    //Initializes list of indexes
    int counter = 0;
    for(int x = 0;x < length;x++) {
        for(int y = 0;y < height;y++) {
            positionList[counter].x = x;
            positionList[counter].y = y;
            counter++;
        }
    }
}

/* Recursively reveals tiles
 * If a tile has 0 surrounding mines, it will reveal surrounding tiles as well*/
void revealAll(tile* game, int length, int height, Position position) {

}

// End of private functions

//Creates a new game
Game* createGame(int length, int height, int mines) {
    Game* game = malloc(sizeof(Game));

    initializeGame(game, length, height, mines);

    return game;
}

//Resets internals of existing game
void resetGame(Game* game, int length, int height, int mines) {
    freeBoard(game->board);
    initializeGame(game, length, height, mines);
}

//Frees all memory associated with game
void deleteGame(Game* game) {
    freeBoard(game->board);
    free(game);
}

//Reveals a tile and all surrounding tiles (if empty)
state reveal(Game* game, int x, int y) {
    Position position;
    position.x = x;
    position.y = y;

    //If this is the first move, mines must be placed
    if(!game->gameStarted) {
        placeMines(game->board, game->length, game->height, position);
    }
}

void flag(Game* game, int x, int y) {
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