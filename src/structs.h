#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "minesweeper.h"

typedef struct {
    SDL_Texture *gameb, *gamew, *beginner, *beginnerSpec, *intermediate, *intermediateSpec, *expert, *expertSpec;
    int buttonWidth, textHeight, boxWidth, boxHeight, button1y, button2y, button3y, gameButtonWidth;
    bool visible, button1, button2, button3;
} OptionsMenu;

typedef struct {
    bool mouseDown,  menuOpen, faceMouseOver, menuMouseOver, gameButtonMouseOver;
    int mouseX, mouseY;
    int boardXEnd, boardYStart, boardYEnd, faceXPosition;
    SDL_Texture *texture, *analogue, *face;
} WindowInfo;

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    Game *game;
    WindowInfo info;
    OptionsMenu options;
    long startTime;
    int timer;
} App;

#endif