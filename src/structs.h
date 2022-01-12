#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "minesweeper.h"

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    Game *game;
    bool mouseDown;
    int x, y;
} App;

#endif