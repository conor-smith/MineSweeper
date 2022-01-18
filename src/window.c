#include <SDL2/SDL_video.h>

#include "defs.h"
#include "structs.h"

extern App app;

void setNewWindowSize() {
    int length = app.info.boardXEnd + PADDING;
    length = length < MIN_WINDOW_WIDTH ? MIN_WINDOW_WIDTH : length;
    length = length < app.options.boxWidth ? app.options.boxWidth : length;

    int height = app.info.boardYEnd + PADDING;

    SDL_SetWindowSize(app.window, length, height);
}