#include <SDL2/SDL_render.h>

#include "structs.h"
#include "enums.h"
#include "minesweeper.h"
#include "defs.h"

#define F_TEXTURE_OFFSET 64 //Only for the face

extern App app;

/* For the texture offsets representing numbers 1 - 8, 1 has an offset of 0, 2 = 1*OFFSET, 3=2*OFFSET, etc
 * These ones are hardcoded for ease of use and readability*/
SDL_Rect flaggedTile = {TILE_SIZE * 8, 0, TILE_SIZE, TILE_SIZE};
SDL_Rect unselectedTile = {TILE_SIZE * 9, 0, TILE_SIZE, TILE_SIZE};
SDL_Rect selectedTile = {TILE_SIZE * 10, 0, TILE_SIZE, TILE_SIZE};
SDL_Rect mine = {TILE_SIZE * 11, 0, TILE_SIZE, TILE_SIZE};
SDL_Rect xTile = {TILE_SIZE * 12, 0, TILE_SIZE, TILE_SIZE};

// These are for the face
SDL_Rect neutral = {0, 0, F_TEXTURE_OFFSET, F_TEXTURE_OFFSET};
SDL_Rect shocked = {F_TEXTURE_OFFSET, 0, F_TEXTURE_OFFSET, F_TEXTURE_OFFSET};
SDL_Rect winning = {F_TEXTURE_OFFSET * 2, 0, F_TEXTURE_OFFSET, F_TEXTURE_OFFSET};
SDL_Rect losing = {F_TEXTURE_OFFSET * 3, 0, F_TEXTURE_OFFSET, F_TEXTURE_OFFSET};
SDL_Rect unselectedFace = {F_TEXTURE_OFFSET * 4, 0, F_TEXTURE_OFFSET, F_TEXTURE_OFFSET};
SDL_Rect selectedFace = {F_TEXTURE_OFFSET * 5, 0, F_TEXTURE_OFFSET, F_TEXTURE_OFFSET};

// This function maps the minesweeper board co-ordinates to the window co-ordinates
void mapToWindow(int *x, int *y) {
	*x = (*x * TILE_SIZE) + PADDING;
	*y = (*y * TILE_SIZE) + app.info.boardYStart;
}

void drawBox(SDL_Rect box) {
	SDL_RenderDrawLine(app.renderer, box.x, box.y, box.x + box.w, box.y);
	SDL_RenderDrawLine(app.renderer, box.x, box.y, box.x, box.y + box.h);
	SDL_RenderDrawLine(app.renderer, box.x + box.w, box.y + box.h, box.x + box.w, box.y);
	SDL_RenderDrawLine(app.renderer, box.x + box.w, box.y + box.h, box.x, box.y + box.h);
}

void drawText(int x, int y, SDL_Texture *text) {
	SDL_Rect dest = {x, y, 0, 0};
	SDL_QueryTexture(text, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(app.renderer, text, NULL, &dest);
}

void drawMenu() {
	SDL_Rect menuBox = {0, app.options.textHeight, app.options.boxWidth, app.options.boxHeight};
	SDL_Rect buttonBox = {MENU_PADDING, 0, app.options.buttonWidth, app.options.textHeight};

	SDL_SetRenderDrawColor(app.renderer, 160, 160, 160, 255);

	SDL_RenderFillRect(app.renderer, &menuBox);
	menuBox.y = menuBox.y + 1;
	menuBox.h = menuBox.h - 1;

	SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);

	drawBox(menuBox);

	SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
	SDL_Rect highlightedButton = {MENU_PADDING, 0, app.options.buttonWidth, app.options.textHeight};
	if(app.options.button1) {
		highlightedButton.y = app.options.button1y;
		SDL_RenderFillRect(app.renderer, &highlightedButton);
	} else if(app.options.button2) {
		highlightedButton.y = app.options.button2y;
		SDL_RenderFillRect(app.renderer, &highlightedButton);
	} else if(app.options.button3) {
		highlightedButton.y = app.options.button3y;
		SDL_RenderFillRect(app.renderer, &highlightedButton);
	} else if(app.options.button4) {
		highlightedButton.y = app.options.button4y;
		SDL_RenderFillRect(app.renderer, &highlightedButton);
	}

	buttonBox.y = app.options.button1y;
	drawText(MENU_PADDING * 2, app.options.button1y, app.options.beginner);
	drawText(MENU_PADDING * 2 + app.options.buttonWidth, app.options.button1y, app.options.beginnerSpec);
	drawBox(buttonBox);

	buttonBox.y = app.options.button2y;
	drawText(MENU_PADDING * 2, app.options.button2y, app.options.intermediate);
	drawText(MENU_PADDING * 2 + app.options.buttonWidth, app.options.button2y, app.options.intermediateSpec);
	drawBox(buttonBox);

	buttonBox.y = app.options.button3y;
	drawText(MENU_PADDING * 2, app.options.button3y, app.options.expert);
	drawText(MENU_PADDING * 2 + app.options.buttonWidth, app.options.button3y, app.options.expertSpec);
	drawBox(buttonBox);

	buttonBox.y = app.options.button4y;
	drawText(MENU_PADDING * 2, app.options.button4y, app.options.custom);
	drawBox(buttonBox);
}

void drawFace() {
	SDL_Rect dest = {app.info.faceXPosition, app.options.textHeight + PADDING, F_TEXTURE_OFFSET, F_TEXTURE_OFFSET};

	SDL_Rect *background = (app.info.faceMouseOver && app.info.mouseDown) ? &selectedFace : &unselectedFace;

	SDL_RenderCopy(app.renderer, app.info.face, background, &dest);

	SDL_Rect *face;

	switch(getGameState(app.game)) {
		case WIN:
			face = &winning;
			break;
		case LOSS:
			face = &losing;
			break;
		default:
			if(app.info.mouseX != -1 && app.info.mouseDown) {
				face = &shocked;
			} else {
				face = &neutral;
			}
			break;
	}

	SDL_RenderCopy(app.renderer, app.info.face, face, &dest);
}

void drawBanner() {
	SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
	
	SDL_Rect optionsBanner = {0, 0, app.info.boardXEnd + PADDING, app.options.textHeight};
	SDL_RenderFillRect(app.renderer, &optionsBanner);

	if(app.info.gameButtonMouseOver) {
		SDL_Rect highlighted = {0, 0, app.options.gameButtonWidth, app.options.textHeight};

		SDL_SetRenderDrawColor(app.renderer, 0, 121, 197, 255);
		SDL_RenderFillRect(app.renderer, &highlighted);
		drawText(0, 0, app.options.gamew);
	} else {
		drawText(0, 0, app.options.gameb);
	}
}

// This is for drawing either of the two 'analogue' displays
void drawAnalogue(int value, int x, int y) {
	SDL_SetRenderDrawColor(app.renderer, 70, 0, 0, 255);

	SDL_Rect display = {x, y, 96, 64};

	SDL_RenderFillRect(app.renderer, &display);

	for(int i = 2;i >= 0;i--) {
		int valueToPrint = value % 10;
		value = value / 10;

		SDL_Rect src = {valueToPrint * TILE_SIZE, 0, 32, 64};
		SDL_Rect dest = {x + i * TILE_SIZE, y, 32, 64};

		SDL_RenderCopy(app.renderer, app.info.analogue, &src, &dest);
	}
}

// This draws a grid over the top of the window
void drawGrid() {
	SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);

	for(int i = 0;i <= getLength(app.game);i++) {
		int xPos = PADDING + (i * TILE_SIZE);
		SDL_RenderDrawLine(app.renderer, xPos, app.info.boardYStart, xPos, app.info.boardYEnd);
	}

	for(int i = 0;i <= getHeight(app.game);i++) {
		int yPos = app.info.boardYStart + (i * TILE_SIZE);
		SDL_RenderDrawLine(app.renderer, PADDING, yPos, app.info.boardXEnd, yPos);
	}
}

// This draws a single tile
void drawTile(int x, int y, tile t, bool mouseOver) {
	mapToWindow(&x, &y);

	// Where the tile will be placed
	SDL_Rect dest = {x, y, TILE_SIZE, TILE_SIZE};

	/* The texture within img/textures.png
	 * This is only for numbers. It will not be used if it's a non-number tile*/
	SDL_Rect src = {0, 0, TILE_SIZE, TILE_SIZE};

	if(t == HIDDEN_MINE && getGameState(app.game) != ACTIVE) {
		switch(getGameState(app.game)) {
			case LOSS:
				SDL_RenderCopy(app.renderer, app.info.texture, &mine, &dest);
				break;
			case WIN:
				SDL_RenderCopy(app.renderer, app.info.texture, &unselectedTile, &dest);
				SDL_RenderCopy(app.renderer, app.info.texture, &flaggedTile, &dest);
				break;
		}
	} else if(t < REVEALED_0) {
		if(getGameState(app.game) != ACTIVE) {
			SDL_RenderCopy(app.renderer, app.info.texture, &unselectedTile, &dest);
		} else if(mouseOver && app.info.mouseDown) {
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
		src.x = (t - REVEALED_1) * TILE_SIZE;

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
			drawTile(i, j, board[i][j], !app.info.menuOpen && i == app.info.mouseX && j == app.info.mouseY);
		}
	}

	drawGrid();

	// Number of unflagged mines left
	int unflaggedMines = getMines(app.game) - getFlagged(app.game);
	drawAnalogue(unflaggedMines > 0 ? unflaggedMines : 0, 10, app.options.textHeight + PADDING);

	//Timer
	drawAnalogue(app.timer, app.info.boardXEnd - 96, app.options.textHeight + PADDING);

	drawFace();

	drawBanner();

	SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);

	SDL_RenderDrawLine(app.renderer, 0, app.options.textHeight, app.info.boardXEnd + PADDING, app.options.textHeight);
	SDL_RenderDrawLine(app.renderer, 0, app.options.textHeight + DISPLAY_BANNER, app.info.boardXEnd + PADDING, app.options.textHeight + DISPLAY_BANNER);

	if(app.info.menuOpen) {
		drawMenu();
	}
	
    SDL_RenderPresent(app.renderer);
}