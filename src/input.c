#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>
#include "structs.h"
#include "minesweeper.h"
#include "globals.h"

void getMouseState(void) {
    SDL_GetMouseState(&app.x, &app.y);

    if( app.x < xBorder || app.x > xEndBorder ||
        app.y < yBorder || app.y > yEndBorder) {

        app.x = -1;

    } else {
        app.x = (app.x - xBorder) / tileSize;
        app.y = (app.y - yBorder) / tileSize;
    }
}

void handleInput(void) {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                exit(0);
            case SDL_MOUSEMOTION:
                getMouseState();
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT) {
                    app.mouseDown = true;
                    getMouseState();
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT) {
                    app.mouseDown = false;
                    getMouseState();

                    reveal(app.game, app.x, app.y);
                } else if(event.button.button == SDL_BUTTON_RIGHT) {
                    getMouseState();

                    flag(app.game, app.x, app.y);
                }
                break;
            default:
                break;
        }
    }
}