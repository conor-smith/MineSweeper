#include <stdbool.h>
#include "enums.h"
#include "minesweeper.h"

typedef struct MineSweeperGame {
    int length, height, mines, flagged, revealed;
    Tile** board;
    GameState gameState;
    bool gameStarted;
} Game;

//Documentation for functions in header file

Game* createGame(int length, int height, int mines) {
    return null;
}

void deleteGame(Game* game) {
    return null;
}

void resetGame(int length, int height, int mines) {
    return null;
}


GameState reveal(int x, int y) {
    return null;
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
GameState getGameState(Game* game) {
    return game->gameState;
}
Tile** getBoard(Game* game) {
    return game->board;
}