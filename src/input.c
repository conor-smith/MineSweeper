#include <SDL2/SDL_events.h>

#include "structs.h"
#include "minesweeper.h"
#include "time_ms.h"
#include "defs.h"

extern App app;

/* This function translates the pixel x,y co-ordinates on screen
 * into x,y co-ordinates for the board */
void getMouseBoardCoordinates(void) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    if( x < PADDING || x > app.info.boardXEnd ||
        y < BOARD_Y_START || y > app.info.boardYEnd) {

        app.info.mouseX = -1;
    } else {
        app.info.mouseX = (x - PADDING) / TILE_SIZE;
        app.info.mouseY = (y - BOARD_Y_START) / TILE_SIZE;
    }
}

bool handleInput(void) {
    SDL_Event event;
    bool updateScreen = false;
    static int oldX, oldY;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                exit(0);
            case SDL_MOUSEMOTION:
                if(app.info.mouseDown) {
                    getMouseBoardCoordinates();
                    if(oldX != app.info.mouseX || oldY != app.info.mouseY) {
                        oldX = app.info.mouseX;
                        oldY = app.info.mouseY;
                        updateScreen = true;
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT) {
                    app.info.mouseDown = true;
                    getMouseBoardCoordinates();

                    oldX = app.info.mouseX;
                    oldY = app.info.mouseY;
                    updateScreen = true;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                // For both the left and right mouse though, we must manually set the updateScreen field
                switch(event.button.button) {
                    case SDL_BUTTON_LEFT:
                        // If the left button is raised, we must reveal the tile under the mouse
                        app.info.mouseDown = false;
                        getMouseBoardCoordinates();

                        // If this is the first move, start the timer
                        if(!getInitialised(app.game) && app.info.mouseX != -1) {
                            app.startTime = getCurrentTime();
                        }

                        reveal(app.game, app.info.mouseX, app.info.mouseY);
                        break;
                    case SDL_BUTTON_RIGHT:
                        //If the right button is raised, we must flag the tile
                        getMouseBoardCoordinates();

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