#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>
#include "structs.h"
#include "minesweeper.h"
#include "globals.h"

void mapToBoard(void) {
    if( app.x < xBorder || app.x > xBorder + tileSize * getLength(app.game) ||
        app.y < yBorder || app.y > yBorder + tileSize * getHeight(app.game)) {

        app.x = -1;

    } else {
        app.x = (app.x - xBorder) / tileSize;
        app.y = (app.y - yBorder) / tileSize;
    }
}

bool handleInput(void) {
    SDL_Event event;
    bool toReturn = false;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                exit(0);
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&app.x, &app.y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                app.mouseDown = true;
                SDL_GetMouseState(&app.x, &app.y);
                break;
            case SDL_MOUSEBUTTONUP:
                app.mouseDown = false;
                toReturn = true;
                SDL_GetMouseState(&app.x, &app.y);
                break;
            default:
                break;
        }
    }

    mapToBoard();

    return toReturn;
}