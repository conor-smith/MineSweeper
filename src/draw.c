#include <SDL2/SDL_render.h>

#include "structs.h"
#include "enums.h"

extern App app;

extern SDL_Texture *texture;

// The offsets for the texture are as follows
// ONE=0, TWO=32, THREE=64, FOUR=96, FIVE=128, SIX=160, SEVEN=192, EIGHT=224, FLAG=256, TILE=288, SELECTED=320, MINE=352, X=384

void drawSquare(int x, int y, tile t) {
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = 32;
	dest.h = 32;

	SDL_Rect src;
	src.w = 32;
	src.h = 32;
	src.y = 0;

	if(t < 10) {
		src.x = 288;
	} else if(t < 20) {
		src.x = 256;
	} else if(t == REVEALED_0) {
		return;
	} else {
		src.x = (t - 21) * 32;
	}

	SDL_RenderCopy(app.renderer, texture, &src, &dest);
}

void drawScene(void) {
    SDL_SetRenderDrawColor(app.renderer, 192, 192, 192, 255);

    SDL_RenderClear(app.renderer);

	drawSquare(10, 10, HIDDEN_0);
	drawSquare(10, 42, FLAGGED_0);
	
	for(int i = 0;i < 9;i++) {
		drawSquare(10, 74 + (i * 32), REVEALED_0 + i);
	}

    SDL_RenderPresent(app.renderer);
}