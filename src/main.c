#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_timer.h>

#include "structs.h"
#include "minesweeper.h"
#include "init.h"
#include "input.h"
#include "draw.h"

extern App app;

int main(void) {
    atexit(cleanup);

    init();

    app.info.updateScreen = true;
    while(1) {
        handleInput();

        drawSceneIfChange();

        SDL_Delay(4);
    }
}