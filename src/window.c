#include <SDL2/SDL_video.h>

#include "defs.h"
#include "structs.h"

extern App app;

void getNewWindowSize(int *length, int *height) {
    *length = app.info.boardXEnd + PADDING;
    *length = *length < MIN_WINDOW_WIDTH ? MIN_WINDOW_WIDTH : *length;

    *height = app.info.boardYEnd + PADDING;
}

// This should onl
void setNewPositionData() {
    app.info.boardXEnd = PADDING + (TILE_SIZE * getLength(app.game));
    app.info.boardYEnd = BOARD_Y_START + (TILE_SIZE * getHeight(app.game));
    app.info.faceXPosition = (app.info.boardXEnd + PADDING) / 2 - 32;
}