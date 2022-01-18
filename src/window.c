#include <SDL2/SDL_video.h>

#include "defs.h"
#include "structs.h"

extern App app;

void setNewWindowSize() {

    app.options.button1 = false;
    app.options.button2 = false;
    app.options.button3 = false;

    app.startTime = -1;
    app.timer = 0;

    app.info.boardXEnd = PADDING + (TILE_SIZE * getLength(app.game));
    app.info.boardYStart = app.options.textHeight + DISPLAY_BANNER + PADDING;
    app.info.boardYEnd = app.info.boardYStart + (TILE_SIZE * getHeight(app.game));
    app.info.faceXPosition = (app.info.boardXEnd + PADDING) / 2 - 32;
    app.info.menuOpen = false;

    int length = app.info.boardXEnd + PADDING;
    length = length < MIN_WINDOW_WIDTH ? MIN_WINDOW_WIDTH : length;
    length = length < app.options.boxWidth ? app.options.boxWidth : length;

    int height = app.info.boardYEnd + PADDING;

    SDL_SetWindowSize(app.window, length, height);
}