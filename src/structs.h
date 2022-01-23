#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "minesweeper.h"

typedef struct {
    SDL_Texture *gameb, *gamew, *beginner, *beginnerSpec, *intermediate, *intermediateSpec, *expert, *expertSpec, *custom, *length, *height, *mines, *numbers[10];
    int buttonWidth, textHeight, boxWidth, boxHeight, button1y, button2y, button3y, button4y,gameButtonWidth;
    int labelWidth, textBoxWitdh, cursor1, cursor2, cursor3, lengthString[2], heightString[2], minesString[3];
    bool button1, button2, button3, button4, customMenu;
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