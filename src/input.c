#include <SDL2/SDL_events.h>

#include "structs.h"
#include "minesweeper.h"
#include "time_ms.h"
#include "defs.h"

extern App app;

static int oldX, oldY;

/* This function translates the pixel x,y co-ordinates on screen
 * into x,y co-ordinates for the board
 * Also checks if the mouse is over the face*/
void getMouseBoardCoordinates(int x, int y) {
    app.info.faceMouseOver = x > app.info.faceXPosition && x < app.info.faceXPosition + 64 &&
        y > app.options.textHeight + PADDING && y < app.options.textHeight + PADDING + 64;

    if( x < PADDING || x > app.info.boardXEnd ||
        y < app.info.boardYStart || y > app.info.boardYEnd) {

        app.info.mouseX = -1;
    } else {
        app.info.mouseX = (x - PADDING) / TILE_SIZE;
        app.info.mouseY = (y - app.info.boardYStart) / TILE_SIZE;
    }
}

void handleMotion(bool *updateScreen) {
    static bool oldFaceMouseOver, oldGameButtonMouseOver;

    int x, y;
    SDL_GetMouseState(&x, &y);

    app.info.gameButtonMouseOver = x >= 0 && x < app.options.gameButtonWidth && y >= 0 && y < app.options.textHeight;
    if(app.info.gameButtonMouseOver != oldGameButtonMouseOver) {
        *updateScreen = true;
        oldGameButtonMouseOver = app.info.gameButtonMouseOver;
    }

    getMouseBoardCoordinates(x, y);

    if(app.info.mouseDown) {
        if(oldX != app.info.mouseX || oldY != app.info.mouseY) {
            oldX = app.info.mouseX;
            oldY = app.info.mouseY;
            *updateScreen = true;
        } else if(app.info.faceMouseOver || app.info.faceMouseOver != oldFaceMouseOver) {
            oldFaceMouseOver = app.info.faceMouseOver;
            *updateScreen = true;
        }
    }
}

void handleLeftButtonDown(bool *updateScreen) {
    app.info.mouseDown = true;

    if(!app.info.menuOpen) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        getMouseBoardCoordinates(x, y);

        oldX = app.info.mouseX;
        oldY = app.info.mouseY;
        *updateScreen = true;
    }
}

void handleLeftButtonUp() {
    // If the left button is raised, we must reveal the tile under the mouse
    app.info.mouseDown = false;

    int x, y;

    if(app.info.menuOpen) {
        SDL_GetMouseState(&x, &y);

        if(!(x >= 0 && x < app.options.boxWidth && y >= app.options.textHeight && y < app.options.textHeight + app.options.boxHeight)) {
            app.info.menuOpen = false;
        }
    } else if(app.info.gameButtonMouseOver) {
        app.info.menuOpen = true;
    } else if(app.info.faceMouseOver) {
        // If user clicks the face, reset game
        app.startTime = -1;
        app.timer = 0;
        newGame(app.game, getLength(app.game), getHeight(app.game), getMines(app.game));
    } else {
        // If this is the first move, start the timer
        if(!getInitialised(app.game) && app.info.mouseX != -1) {
            app.startTime = getCurrentTime();
        }

        SDL_GetMouseState(&x, &y);
        getMouseBoardCoordinates(x, y);

        reveal(app.game, app.info.mouseX, app.info.mouseY);
    }
}

bool handleInput(void) {
    SDL_Event event;
    bool updateScreen = false;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                exit(0);
            case SDL_MOUSEMOTION:
                handleMotion(&updateScreen);
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT) {
                    handleLeftButtonDown(&updateScreen);
                }
                break;
            case SDL_MOUSEBUTTONUP:
                int x, y;
                SDL_GetMouseState(&x, &y);
                getMouseBoardCoordinates(x, y);
                // For both the left and right mouse though, we must manually set the updateScreen field
                switch(event.button.button) {
                    case SDL_BUTTON_LEFT:
                        handleLeftButtonUp();
                        break;
                    case SDL_BUTTON_RIGHT:
                        //If the right button is raised, we must flag the tile
                        flag(app.game, app.info.mouseX, app.info.mouseY);
                        break;
                }
                updateScreen = true;
                oldX = -1;
                oldY = -1;
                break;
            default:
                break;
        }
    }

    return updateScreen;
}

bool updateTimer(void) {
    if(app.startTime == -1 || getGameState(app.game) != ACTIVE) {
        return false;
    }

    long currentTime = getCurrentTime();
    int difference = (currentTime - app.startTime) / 1000;

    if(difference != app.timer) {
        app.timer = difference;
        return true;
    } else {
        return false;
    }
}