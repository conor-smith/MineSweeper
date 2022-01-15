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

    drawScene();
    while(1) {

        if(handleInput() || updateTimer()) {
            drawScene();
        }

        SDL_Delay(4);
    }
}