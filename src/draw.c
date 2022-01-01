#include <SDL2/SDL_render.h>

#include "structs.h"
#include "enums.h"

extern App app;

extern SDL_Texture *selectedTile;
extern SDL_Texture *unselectedTile;

void drawSquare(int x, int y) {
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(selectedTile, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(app.renderer, selectedTile, NULL, &dest);
}

void drawScene(void) {
    SDL_SetRenderDrawColor(app.renderer, 192, 192, 192, 255);

    SDL_RenderClear(app.renderer);

    drawSquare(10, 10);

    SDL_RenderPresent(app.renderer);
}