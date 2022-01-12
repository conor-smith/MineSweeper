#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>

#include "structs.h"
#include "minesweeper.h"

App app;

void setUpWindowMetadata(void) {
    app.info.boardXBegin = 10;
    app.info.boardYBegin = 10;
    app.info.tileSize = 32;
    app.info.boardXEnd = app.info.boardXBegin + (app.info.tileSize * getLength(app.game));
    app.info.boardYEnd = app.info.boardYBegin + (app.info.tileSize * getHeight(app.game));
}

// Just a big ol' grand initialization of everything
void init(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    app.game = createGame(16, 16, 40);
    
    setUpWindowMetadata();

    int width = getLength(app.game) * app.info.tileSize + app.info.boardXBegin * 2;
    int height = getHeight(app.game) * app.info.tileSize + app.info.boardYBegin * 2;

    app.window = SDL_CreateWindow(
        "MineSweeper", // Window title
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, // Window position x and y. Undefined so window manager decides
        width, height, // Window height and width
        0); // Flags. None are needed

    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    IMG_Init(IMG_INIT_PNG);
    app.info.texture = IMG_LoadTexture(app.renderer, "img/textures.png");

    if(app.info.texture == NULL) {
        printf("%s\n", SDL_GetError());
        exit(1);
    }
}

void cleanup(void) {
    SDL_DestroyRenderer(app.renderer);

    SDL_DestroyWindow(app.window);

    deleteGame(app.game);
}