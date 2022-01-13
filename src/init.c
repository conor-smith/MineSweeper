#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>

#include "structs.h"
#include "minesweeper.h"
#include "window.h"

App app;

// This must be called after the game is initialized
void setUpWindowMetadata(void) {
    app.info.boardXBegin = BORDER;
    app.info.boardYBegin = BANNER_HEIGHT;
    app.info.tileSize = 32;
    app.info.boardXEnd = app.info.boardXBegin + (app.info.tileSize * getLength(app.game));
    app.info.boardYEnd = app.info.boardYBegin + (app.info.tileSize * getHeight(app.game));
}

// THis also must be called after metadata initialization
void setUpSDL(void) {
    int width, height;
    getWindowSize(&width, &height);

    app.window = SDL_CreateWindow(
        "MineSweeper", // Window title
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, // Window position x and y. Undefined so window manager decides
        width, height, // Window height and width
        0); // Flags. None are needed

    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Surface *icon = IMG_Load("img/icon.png");

    SDL_SetWindowIcon(app.window, icon);
}

// This must be called after the window and renderer are initialized
void setUpTextures(void) {
    app.info.texture = IMG_LoadTexture(app.renderer, "img/textures.png");
    app.info.analogue = IMG_LoadTexture(app.renderer, "img/analogue.png");
}

// Just a big ol' grand initialization of everything
void init(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    IMG_Init(IMG_INIT_PNG);

    app.game = createGame(16, 16, 40);
    app.startTime = -1;
    
    setUpWindowMetadata();

    setUpSDL();

    setUpTextures();

    if(app.info.texture == NULL) {
        printf("%s\n", SDL_GetError());
        exit(1);
    }
}

// While not technically necessary, still safer and ultimately helps catch errors
void cleanup(void) {
    SDL_DestroyRenderer(app.renderer);

    SDL_DestroyWindow(app.window);

    deleteGame(app.game);
}