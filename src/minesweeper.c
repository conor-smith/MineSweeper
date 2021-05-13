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

//Documentation for public functions in header file

Game* createGame(int length, int height, int mines) {
    return malloc(sizeof(Game));
}

void deleteGame(Game* game) {
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