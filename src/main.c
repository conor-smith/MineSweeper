#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_timer.h>

#include "structs.h"
#include "minesweeper.h"
#include "init.h"
#include "input.h"

App app;

int main(void) {
    atexit(cleanup);

    initSDL();

    while(1) {
        handleInput();

        SDL_Delay(16);
    }
}