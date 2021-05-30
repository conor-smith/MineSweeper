#ifndef TILE_H
#define TILE_H

//A simple enum to keep track of game state
typedef enum {LOSS=-1, ACTIVE=0, WIN=1} state;

//Each tile has a total of 30 possible states, all of which are represented here. 
typedef enum { HIDDEN_0=0, HIDDEN_1=1, HIDDEN_2=2, HIDDEN_3=3, HIDDEN_4=4, HIDDEN_5=5, HIDDEN_6=6, HIDDEN_7=7, HIDDEN_8=8, HIDDEN_MINE=9,
            FLAGGED_0=10, FLAGGED_1=11, FLAGGED_2=12, FLAGGED_3=13, FLAGGED_4=14, FLAGGED_5=15, FLAGGED_6=16, FLAGGED_7=17, FLAGGED_8=18, FLAGGED_MINE=19,
            REVEALED_0=20, REVEALED_1=21, REVEALED_2=22, REVEALED_3=23, REVEALED_4=24, REVEALED_5=25, REVEALED_6=26, REVEALED_7=27, REVEALED_8=28, REVEALED_MINE=29} tile;

#endif