#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "minesweeper.h"

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    Game *game;
} App;