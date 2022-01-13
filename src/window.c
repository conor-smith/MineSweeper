#include "structs.h"
#include "minesweeper.h"
#include "window.h"

extern App app;

void getWindowSize(int *x, int *y) {
    int width = getLength(app.game) * app.info.tileSize + (BORDER * 2);
    *x = width <= MIN_WIDTH ? MIN_WIDTH : width;

    *y = BANNER_HEIGHT + (getHeight(app.game) * app.info.tileSize) + BORDER;
}