#include <SDL2/SDL_events.h>

#include "structs.h"
#include "minesweeper.h"
#include "time_ms.h"

extern App app;

/* This function translates the pixel x,y co-ordinates on screen
 * into x,y co-ordinates for the board */
void getMouseBoardCoordinates(void) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    if( x < app.info.boardXBegin || x > app.info.boardXEnd ||
        y < app.info.boardYBegin || y > app.info.boardYEnd) {

        app.info.mouseX = -1;
    } else {
        app.info.mouseX = (x - app.info.boardXBegin) / app.info.tileSize;
        app.info.mouseY = (y - app.info.boardYBegin) / app.info.tileSize;
    }
}

void handleInput(void) {
    SDL_Event event;
    int x,y;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                exit(0);
            case SDL_MOUSEMOTION:
                getMouseBoardCoordinates();
                break;
            case SDL_MOUSEBUTTONDOWN:
                /* The drawScene function will automatically update the screen if the user
                 * holds down the mouse while moving it across tiles
                 * and there is no need to set updateScreen to true here*/
                if(event.button.button == SDL_BUTTON_LEFT) {
                    app.info.mouseDown = true;
                    getMouseBoardCoordinates();
                }
                break;
            case SDL_MOUSEBUTTONUP:
                // For both the left and right mouse though, we must manually set the updateScreen field
                if(event.button.button == SDL_BUTTON_LEFT) {
                    // If the left button is raised, we must reveal the tile under the mouse
                    app.info.mouseDown = false;
                    getMouseBoardCoordinates();

                    // If this is the first move, start the timer
                    if(!getInitialised(app.game) && app.info.mouseX != -1) {
                        app.startTime = getCurrentTime();
                    }

                    reveal(app.game, app.info.mouseX, app.info.mouseY);
                } else if(event.button.button == SDL_BUTTON_RIGHT) {
                    //If the right button is raised, we must flag the tile
                    getMouseBoardCoordinates();

                    flag(app.game, app.info.mouseX, app.info.mouseY);
                }
                app.info.updateScreen = true;
                break;
            default:
                break;
        }
    }
}