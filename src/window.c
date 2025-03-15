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
    app.info.menuOpen = false;

    int length = app.info.boardXEnd + PADDING;
    int height = app.info.boardYEnd + PADDING;

    app.info.faceXPosition = length / 2 - 32; // thing
    app.info.analogue2Xposition = length - PADDING - (32 * 3);

    SDL_SetWindowSize(app.window, length, height);


    // This is a change
}