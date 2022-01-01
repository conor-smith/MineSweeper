#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "structs.h"
#include "minesweeper.h"

extern App app;

void initSDL(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    app.window = SDL_CreateWindow(
        "MineSweeper", // Window title
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, // Window position x and y
        500, 500, // Window height and width
        0); // Flags. None are needed

    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    app.game = createGame(10, 10, 10);
}

void cleanup(void) {
    SDL_DestroyRenderer(app.renderer);

    SDL_DestroyWindow(app.window);

    deleteGame(app.game);
}