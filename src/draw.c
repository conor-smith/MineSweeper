#include <SDL2/SDL_render.h>

#include "structs.h"
#include "enums.h"

extern App app;

extern SDL_Texture *texture;

typedef enum offset {ONE=0, TWO=1, THREE=2, FOUR=3, FIVE=4, SIX=5, SEVEN=6, EIGHT=7, FLAG=8, TILE=9, SELECTED=10, MINE=11, X=12};

void drawSquare(int x, int y) {
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    

	SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

void drawScene(void) {
    SDL_SetRenderDrawColor(app.renderer, 192, 192, 192, 255);

    SDL_RenderClear(app.renderer);

    drawSquare(10, 10);

    SDL_RenderPresent(app.renderer);
}