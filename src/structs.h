#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "minesweeper.h"

typedef struct {
    bool mouseDown, faceMouseOver;
    int mouseX, mouseY;
    int boardXEnd, boardYEnd, faceXPosition;
    SDL_Texture *texture, *analogue, *face;
} WindowInfo;

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    Game *game;
    WindowInfo info;
    long startTime;
    int timer;
} App;

#endif