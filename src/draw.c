#include <SDL2/SDL_render.h>

#include "structs.h"
#include "enums.h"
#include "minesweeper.h"
#include "globals.h"

// The offsets for the texture are as follows
// ONE=0, TWO=32, THREE=64, FOUR=96, FIVE=128, SIX=160, SEVEN=192, EIGHT=224, FLAG=256, TILE=288, SELECTED=320, MINE=352, X=384

int oldX, oldY;
bool mouseDown;

void mapToWindow(int *x, int *y) {
	*x = (*x * tileSize) + xBorder;
	*y = (*y * tileSize) + yBorder; 
}

void drawTile(int x, int y, tile t, bool selected) {
	mapToWindow(&x, &y);

	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = tileSize;
	dest.h = tileSize;

	// Texture is of fixed size
	// These values are hard coded
	SDL_Rect src;
	src.y = 0;
	src.w = 32;
	src.h = 32;

	if(t < 20) {
		src.x = selected ? 320 : 288;

		SDL_RenderCopy(app.renderer, texture, &src, &dest);

		if(t < 20 && t >= 10) {
			src.x = 256;

			SDL_RenderCopy(app.renderer, texture, &src, &dest);

			if(getGameState(app.game) == LOSS && t != FLAGGED_MINE) {
				src.x = 384;

				SDL_RenderCopy(app.renderer, texture, &src, &dest);
			}
		}

		if(getGameState(app.game) == LOSS && t == HIDDEN_MINE) {
			src.x = 352;

			SDL_RenderCopy(app.renderer, texture, &src, &dest);
		}
	} else if(t > 20) {
		src.x = (t - 21) * 32;

		SDL_RenderCopy(app.renderer, texture, &src, &dest);
	}
}

void drawGrid() {
	SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);

	for(int i = 0;i <= getLength(app.game);i++) {
		int xPos = xBorder + (i * tileSize);
		SDL_RenderDrawLine(app.renderer, xPos, yBorder, xPos, yEndBorder);
	}

	for(int i = 0;i <= getHeight(app.game);i++) {
		int yPos = yBorder + (i * tileSize);
		SDL_RenderDrawLine(app.renderer, xBorder, yPos, xEndBorder, yPos);
	}
}

void drawScene() {
	oldX = app.x;
	oldY = app.y;
	mouseDown = app.mouseDown;

    SDL_SetRenderDrawColor(app.renderer, 192, 192, 192, 255);

    SDL_RenderClear(app.renderer);

	tile **board = getBoard(app.game);

	for(int i = 0;i < getLength(app.game);i++) {
		for(int j = 0;j < getHeight(app.game); j++) {
			drawTile(i, j, board[i][j], i == app.x && j == app.y);
		}
	}

	drawGrid();
	
    SDL_RenderPresent(app.renderer);
}

void drawSceneIfChange() {
	if(oldX != app.x || oldY != app.y || mouseDown != app.mouseDown) {
		drawScene();
	}
}