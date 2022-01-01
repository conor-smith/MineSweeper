#include <SDL2/SDL_render.h>

#include "structs.h"
#include "enums.h"

extern App app;

void drawScene(void) {
    SDL_SetRenderDrawColor(app.renderer, 192, 192, 192, 255);

    SDL_RenderClear(app.renderer);

    SDL_RenderPresent(app.renderer);
}