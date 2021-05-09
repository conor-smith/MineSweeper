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

//Frees memory
void deleteGame(Game* game);

//Resets board in existing game
void resetGame(int length, int height, int mines);

/* Used to play the game
 * Will print to stdout if invalid parameters are given, and ignore invalid move
 * Will ignore any any attempts to reveal an already revealed or flagged tile, and will ignore attempts to reveal flagged tiles 
 * Once game has been won or lost, all subsequent moves are ignored*/
enum GameState reveal(int x, int y);
void flag(int x, int y);

//Returns game metadata
int getLength(Game* game);
int getHeight(Game* game);
int getMines(Game* game);
int getFlagged(Game* game);
int getRevealed(Game* game);
bool getGameStarted(Game* game);
enum GameState getGameState(Game* game);
enum Tile** getBoard(Game* game);

#endif