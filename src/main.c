#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_timer.h>

#include "structs.h"
#include "minesweeper.h"
#include "init.h"
#include "input.h"
#include "draw.h"

App app;

SDL_Texture *texture;

int main(void) {
    atexit(cleanup);

    initSDL();

    drawScene();

    while(1) {
        handleInput();

        SDL_Delay(16);
    }
}