#include <SDL2/SDL_render.h>

#include "structs.h"
#include "enums.h"
#include "minesweeper.h"

#define TEXTURE_OFFSET 32 // Applies to both board and analogue

extern App app;

/* For the texture offsets representing numbers 1 - 8, 1 has an offset of 0, 2 = 1*OFFSET, 3=2*OFFSET, etc
 * These ones are hardcoded for ease of use and readability*/
SDL_Rect flaggedTile = {TEXTURE_OFFSET * 8, 0, TEXTURE_OFFSET, TEXTURE_OFFSET};
SDL_Rect unselectedTile = {TEXTURE_OFFSET * 9, 0, TEXTURE_OFFSET, TEXTURE_OFFSET};
SDL_Rect selectedTile = {TEXTURE_OFFSET * 10, 0, TEXTURE_OFFSET, TEXTURE_OFFSET};
SDL_Rect mine = {TEXTURE_OFFSET * 11, 0, TEXTURE_OFFSET, TEXTURE_OFFSET};
SDL_Rect xTile = {TEXTURE_OFFSET * 12, 0, TEXTURE_OFFSET, TEXTURE_OFFSET};

/* These keep track of the mouse on the board
 * if the player is holding down the mouse, these ensure the board will only be refreshed when the mouse is moved*/
int oldX, oldY;

// This function maps the minesweeper board co-ordinates to the window co-ordinates
void mapToWindow(int *x, int *y) {
	*x = (*x * app.info.tileSize) + app.info.boardXBegin;
	*y = (*y * app.info.tileSize) + app.info.boardYBegin; 
}

void drawAnalogue(int value, int x, int y) {
	SDL_SetRenderDrawColor(app.renderer, 70, 0, 0, 255);

	SDL_Rect display = {x, y, 96, 64};

	SDL_RenderFillRect(app.renderer, &display);

	for(int i = 2;i >= 0;i--) {
		int valueToPrint = value % 10;
		value = value / 10;

		SDL_Rect src = {valueToPrint * TEXTURE_OFFSET, 0, 32, 64};
		SDL_Rect dest = {x + i * TEXTURE_OFFSET, y, 32, 64};

		SDL_RenderCopy(app.renderer, app.info.analogue, &src, &dest);
	}
}

// This draws a grid over the top of the window
void drawGrid() {
	SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);

	for(int i = 0;i <= getLength(app.game);i++) {
		int xPos = app.info.boardXBegin + (i * app.info.tileSize);
		SDL_RenderDrawLine(app.renderer, xPos, app.info.boardYBegin, xPos, app.info.boardYEnd);
	}

	for(int i = 0;i <= getHeight(app.game);i++) {
		int yPos = app.info.boardYBegin + (i * app.info.tileSize);
		SDL_RenderDrawLine(app.renderer, app.info.boardXBegin, yPos, app.info.boardXEnd, yPos);
	}
}

// This draws a single tile
void drawTile(int x, int y, tile t, bool mouseOver) {
	mapToWindow(&x, &y);

	// Where the tile will be placed
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = app.info.tileSize;
	dest.h = app.info.tileSize;

	/* The texture within img/textures.png
	 * This is only for numbers. It will not be used if it's a non-number tile*/
	SDL_Rect src;
	src.y = 0;
	src.w = TEXTURE_OFFSET;
	src.h = TEXTURE_OFFSET;

	if(t < 20) {
		if(mouseOver && app.info.mouseDown) {
			SDL_RenderCopy(app.renderer, app.info.texture, &selectedTile, &dest);
		} else {
			SDL_RenderCopy(app.renderer, app.info.texture, &unselectedTile, &dest);
		}

		if(t < 20 && t >= 10) {
			SDL_RenderCopy(app.renderer, app.info.texture, &flaggedTile, &dest);

			if(getGameState(app.game) == LOSS && t != FLAGGED_MINE) {
				SDL_RenderCopy(app.renderer, app.info.texture, &xTile, &dest);
			}
		}

		if(getGameState(app.game) == LOSS && t == HIDDEN_MINE) {
			SDL_RenderCopy(app.renderer, app.info.texture, &mine, &dest);
		}
	} else if(t > 20 && t < REVEALED_MINE) {
		src.x = (t - 21) * TEXTURE_OFFSET;

		SDL_RenderCopy(app.renderer, app.info.texture, &src, &dest);
	} else if(t == REVEALED_MINE && getGameState(app.game) == LOSS) {
		SDL_RenderCopy(app.renderer, app.info.texture, &mine, &dest);
		SDL_RenderCopy(app.renderer, app.info.texture, &xTile, &dest);
	}
}

void drawScene() {
	oldX = app.info.mouseX;
	oldY = app.info.mouseY;

    SDL_SetRenderDrawColor(app.renderer, 192, 192, 192, 255);

    SDL_RenderClear(app.renderer);

	tile **board = getBoard(app.game);

	for(int i = 0;i < getLength(app.game);i++) {
		for(int j = 0;j < getHeight(app.game); j++) {
			drawTile(i, j, board[i][j], i == app.info.mouseX && j == app.info.mouseY);
		}
	}

	drawGrid();

	drawAnalogue(124, 10, 10);
	
    SDL_RenderPresent(app.renderer);
}

void drawSceneIfChange() {
	if(oldX != app.info.mouseX || oldY != app.info.mouseY || app.info.updateScreen) {
		drawScene();
	}
}