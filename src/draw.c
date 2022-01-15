#include <SDL2/SDL_render.h>

#include "structs.h"
#include "enums.h"
#include "minesweeper.h"
#include "defs.h"

#define TEXTURE_OFFSET 32 // Applies to both board and analogue

extern App app;

/* For the texture offsets representing numbers 1 - 8, 1 has an offset of 0, 2 = 1*OFFSET, 3=2*OFFSET, etc
 * These ones are hardcoded for ease of use and readability*/
SDL_Rect flaggedTile = {TEXTURE_OFFSET * 8, 0, TEXTURE_OFFSET, TEXTURE_OFFSET};
SDL_Rect unselectedTile = {TEXTURE_OFFSET * 9, 0, TEXTURE_OFFSET, TEXTURE_OFFSET};
SDL_Rect selectedTile = {TEXTURE_OFFSET * 10, 0, TEXTURE_OFFSET, TEXTURE_OFFSET};
SDL_Rect mine = {TEXTURE_OFFSET * 11, 0, TEXTURE_OFFSET, TEXTURE_OFFSET};
SDL_Rect xTile = {TEXTURE_OFFSET * 12, 0, TEXTURE_OFFSET, TEXTURE_OFFSET};

// This function maps the minesweeper board co-ordinates to the window co-ordinates
void mapToWindow(int *x, int *y) {
	*x = (*x * TILE_SIZE) + PADDING;
	*y = (*y * TILE_SIZE) + BOARD_Y_START;
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
		int xPos = PADDING + (i * TILE_SIZE);
		SDL_RenderDrawLine(app.renderer, xPos, BOARD_Y_START, xPos, app.info.boardYEnd);
	}

	for(int i = 0;i <= getHeight(app.game);i++) {
		int yPos = BOARD_Y_START + (i * TILE_SIZE);
		SDL_RenderDrawLine(app.renderer, PADDING, yPos, app.info.boardXEnd, yPos);
	}
}

// This draws a single tile
void drawTile(int x, int y, tile t, bool mouseOver) {
	mapToWindow(&x, &y);

	// Where the tile will be placed
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = TILE_SIZE;
	dest.h = TILE_SIZE;

	/* The texture within img/textures.png
	 * This is only for numbers. It will not be used if it's a non-number tile*/
	SDL_Rect src;
	src.y = 0;
	src.w = TEXTURE_OFFSET;
	src.h = TEXTURE_OFFSET;

	if(getGameState(app.game) == LOSS && t == HIDDEN_MINE) {
		SDL_RenderCopy(app.renderer, app.info.texture, &mine, &dest);
	} else if(t < REVEALED_0) {
		if(mouseOver && app.info.mouseDown) {
			SDL_RenderCopy(app.renderer, app.info.texture, &selectedTile, &dest);
		} else {
			SDL_RenderCopy(app.renderer, app.info.texture, &unselectedTile, &dest);
		}

		if(t >= FLAGGED_0) {
			SDL_RenderCopy(app.renderer, app.info.texture, &flaggedTile, &dest);

			if(getGameState(app.game) == LOSS && t != FLAGGED_MINE) {
				SDL_RenderCopy(app.renderer, app.info.texture, &xTile, &dest);
			}
		}
	} else if(t > REVEALED_0 && t < REVEALED_MINE) {
		src.x = (t - REVEALED_1) * TEXTURE_OFFSET;

		SDL_RenderCopy(app.renderer, app.info.texture, &src, &dest);
	} else if(t == REVEALED_MINE) {
		SDL_RenderCopy(app.renderer, app.info.texture, &mine, &dest);
		SDL_RenderCopy(app.renderer, app.info.texture, &xTile, &dest);
	}
}

void drawScene() {

    SDL_SetRenderDrawColor(app.renderer, 192, 192, 192, 255);

    SDL_RenderClear(app.renderer);

	tile **board = getBoard(app.game);

	for(int i = 0;i < getLength(app.game);i++) {
		for(int j = 0;j < getHeight(app.game); j++) {
			drawTile(i, j, board[i][j], i == app.info.mouseX && j == app.info.mouseY);
		}
	}

	drawGrid();

	// Number of unflagged mines left
	int unflaggedMines = getMines(app.game) - getFlagged(app.game);
	drawAnalogue(unflaggedMines > 0 ? unflaggedMines : 0, 10, 26);

	//Timer
	drawAnalogue(app.timer, app.info.boardXEnd - 96, 26);
	
    SDL_RenderPresent(app.renderer);
}