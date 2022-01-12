#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>

#include "structs.h"
#include "minesweeper.h"
#include "globals.h"

int xBorder;
int yBorder;
int tileSize;
int xEndBorder;
int yEndBorder;
SDL_Texture *texture;
App app;

void setUpGlobals(void) {
    xBorder = 10;
    yBorder = 10;
    tileSize = 32;
    xEndBorder = xBorder + (tileSize * getLength(app.game));
    yEndBorder = yBorder + (tileSize * getHeight(app.game));
}

void init(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    app.game = createGame(16, 16, 40);
    
    setUpGlobals();

    int width = getLength(app.game) * tileSize + xBorder * 2;
    int height = getHeight(app.game) * tileSize + yBorder * 2;

    app.window = SDL_CreateWindow(
        "MineSweeper", // Window title
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, // Window position x and y. Undefined so window manager decides
        width, height, // Window height and width
        0); // Flags. None are needed

    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    IMG_Init(IMG_INIT_PNG);
    texture = IMG_LoadTexture(app.renderer, "img/textures.png");

    if(texture == NULL) {
        printf("%s\n", SDL_GetError());
        exit(1);
    }
}

void cleanup(void) {
    SDL_DestroyRenderer(app.renderer);

    SDL_DestroyWindow(app.window);

    deleteGame(app.game);
}