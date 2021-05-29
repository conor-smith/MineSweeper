#include <stdlib.h>
#include <stdbool.h>
#include "enums.h"
#include "minesweeper.h"

typedef struct MineSweeperGame {
    int length, height, mines, flagged, revealed;
    enum Tile** board;
    enum GameState gameState;
    bool gameStarted;
} Game;

//These are 'private' functions. They are for internal use and are not declared in the header file

/* Returns double pointer of tiles
 * Creates an array of size length * height, then assigns it to a pointer
 * Assigns subsequent pointers to multiples of length, creating a 2D array*/
enum Tile** createBoard(int length, int height) {
    enum Tile** board = malloc(length * sizeof(enum Tile*));
    enum Tile* tileArray = calloc(length * height, sizeof(enum Tile));

    for(int i = 0;i < length;i++) {
        board[i] = tileArray + length * i;
    }

    return board;
}

// Frees the board specificaly
void freeBoard(enum Tile** board) {
    free(board[0]);
    free(board);
}

//All following functions are declared in header file

Game* createGame(int length, int height, int mines) {
    length = length >= 1 ? length : 1;
    height = height >= 1 ? height : 1;
    mines = mines >= 0 ? mines : 0;
    mines = mines <= length * height ? mines : length * height;

    Game* game = malloc(sizeof(Game));
    game->length = length;
    game->height = height;
    game->mines = mines;
    game->flagged = 0;
    game->revealed = 0;
    game->gameState = ACTIVE;
    game->gameStarted = false;
    game->board = createBoard(length, height);

    return game;
}

void deleteGame(Game* game) {
    freeBoard(game->board);
    free(game);
}

void resetGame(int length, int height, int mines) {
}


enum GameState reveal(int x, int y) {
    return ACTIVE;
}

void flag(int x, int y) {
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
enum GameState getGameState(Game* game) {
    return game->gameState;
}
enum Tile** getBoard(Game* game) {
    return game->board;
}