#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <stdbool.h>
#include "enums.h"

//All game data will be represented in this struct
typedef struct MineSweeperGame Game;

/* Creates game object
 * Prints to stdout if an invalid entry is given (eg, more mines than tiles, negative height value, etc)
 * When this happens, The offending parameter will be replaced with the nearest valid value (eg, -1 length = 1. -1 mines = 0) */
Game* createGame(int length, int height, int mines);
Game* createEmptyGame();

//Creates new game using existing game struct
void newGame(Game* game, int length, int height, int mines);

//Recreates board exactly as it was before. Ignores if game hasn't been initialized
void resetGame(Game* game);

//Frees memory
void deleteGame(Game* game);

/* Used to play the game
 * The board will not be built until the first reveal occurs
 * Will print to stdout if invalid parameters are given, and ignore invalid move
 * Will ignore any any attempts to reveal an already revealed or flagged tile, and will ignore attempts to flag revealled tiles 
 * Once game has been won or lost, all subsequent moves are ignored*/
state reveal(Game* game, int x, int y);
void flag(Game* game, int x, int y);

//Returns game data. In java, these would be getters
int getLength(Game* game);
int getHeight(Game* game);
int getMines(Game* game);
int getFlagged(Game* game);
int getRevealed(Game* game);
bool getInitialised(Game* game);
state getGameState(Game* game);
tile** getBoard(Game* game);

#endif