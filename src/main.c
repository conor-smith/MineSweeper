#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_timer.h>

#include "structs.h"
#include "minesweeper.h"
#include "init.h"
#include "input.h"
#include "draw.h"

int main(void) {
    atexit(cleanup);

    init();

    drawScene();
    while(1) {
        handleInput();

        drawSceneIfChange();

        SDL_Delay(4);
    }
}