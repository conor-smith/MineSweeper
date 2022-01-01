#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_timer.h>

#include "structs.h"
#include "minesweeper.h"
#include "init.h"
#include "input.h"
#include "draw.h"

App app;

SDL_Texture *unselectedTile;
SDL_Texture *selectedTile;

int main(void) {
    atexit(cleanup);

    initSDL();

    while(1) {
        handleInput();

        drawScene();

        SDL_Delay(16);
    }
}