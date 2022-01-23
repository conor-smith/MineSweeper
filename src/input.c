#include <SDL2/SDL_events.h>

#include "structs.h"
#include "minesweeper.h"
#include "time_ms.h"
#include "defs.h"
#include "window.h"

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

void getButton(int x, int y, bool *updateScreen) {
    static bool wasOverButton;

    app.options.button1 = false;
    app.options.button2 = false;
    app.options.button3 = false;
    app.options.button4 = false;

    if(x >= MENU_PADDING && x < MENU_PADDING + app.options.buttonWidth) {
        if(y >= app.options.button1y && y < app.options.button2y) {
            app.options.button1 = true;
            *updateScreen = true;
            wasOverButton = true;
        } else if(y >= app.options.button2y && y < app.options.button3y) {
            app.options.button2 = true;
            *updateScreen = true;
            wasOverButton = true;
        } else if(y >= app.options.button3y && y < app.options.button4y) {
            app.options.button3 = true;
            *updateScreen = true;
            wasOverButton = true;
        }else if(y >= app.options.button4y && y < app.options.button4y + app.options.textHeight) {
            app.options.button4 = true;
            *updateScreen = true;
            wasOverButton = true;
        } else if(wasOverButton) {
            wasOverButton = false;
            *updateScreen = true;
        }
    } else if(wasOverButton) {
        wasOverButton = false;
        *updateScreen = true;
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

    if(app.info.menuOpen && !app.options.customMenu) {
        getButton(x, y, updateScreen);
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

// No need for updateScreen here. The screen will always update after a click
void handleLeftButtonUp() {
    // If the left button is raised, we must reveal the tile under the mouse
    app.info.mouseDown = false;

    int x, y;

    if(app.info.menuOpen) {
        SDL_GetMouseState(&x, &y);

        if(!(x >= 0 && x < app.options.boxWidth && y >= app.options.textHeight && y < app.options.textHeight + app.options.boxHeight)) {
            app.info.menuOpen = false;
            app.options.customMenu = false;
        } else if(app.options.customMenu) {
            app.options.button1 = false;
            app.options.button2 = false;
            app.options.button3 = false;
            if(x > app.options.labelWidth + MENU_PADDING * 2 && x < app.options.labelWidth + MENU_PADDING * 2 + app.options.textBoxWitdh) {
                if(y > app.options.button1y && y < app.options.button2y) {
                    app.options.button1 = true;
                } else if(y > app.options.button2y && y < app.options.button3y) {
                    app.options.button2 = true;
                } else if(y > app.options.button3y && y < app.options.button4y) {
                    app.options.button3 = true;
                }
            } 
        } else {
            bool throwAway;
            getButton(x, y, &throwAway);

            if(app.options.button1) {
                newGame(app.game, 9, 9, 16);
                setNewWindowSize();
            } else if(app.options.button2) {
                newGame(app.game, 16, 16, 40);
                setNewWindowSize();
            } else if(app.options.button3) {
                newGame(app.game, 30, 16, 99);
                setNewWindowSize();
            } else if(app.options.button4) {
                app.options.customMenu = true;
            }
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

int power(int base, int power) {
    int toReturn = 1;
    for(int i = 0;i < power;i++) {
        toReturn *= base;
    }

    return toReturn;
}

int parseTextBox(int cursor, int *textBox) {
    int toReturn = 0;
    int cursorOriginalValue = cursor;
    for(int i = 0;i< cursorOriginalValue;i++) {
        cursor--;
        toReturn += power(10, cursor) * textBox[i];
    }
    return toReturn;
}

// It's late and I'm too tired to optimize
void handleKeyboard(SDL_KeyboardEvent *event, bool *updateScreen) {
    if(app.options.customMenu) {
        if(app.options.button1) {
            if(event->keysym.scancode == SDL_SCANCODE_BACKSPACE && app.options.cursor1 > 0){
                app.options.cursor1--;
            } else if(event->keysym.scancode == SDL_SCANCODE_0 && app.options.cursor1 < 2) {
                app.options.lengthString[app.options.cursor1] = 0;
                app.options.cursor1++;
            } else if(event->keysym.scancode < SDL_SCANCODE_0 && event->keysym.scancode >= SDL_SCANCODE_1 && app.options.cursor1 < 2) {
                app.options.lengthString[app.options.cursor1] = event->keysym.scancode - SDL_SCANCODE_1 + 1;
                app.options.cursor1++;
            }
        } else if(app.options.button2) {
            if(event->keysym.scancode == SDL_SCANCODE_BACKSPACE && app.options.cursor2 > 0){
                app.options.cursor2--;
            } else if(event->keysym.scancode == SDL_SCANCODE_0 && app.options.cursor2 < 2) {
                app.options.heightString[app.options.cursor2] = 0;
                app.options.cursor2++;
            } else if(event->keysym.scancode < SDL_SCANCODE_0 && event->keysym.scancode >= SDL_SCANCODE_1 && app.options.cursor2 < 2) {
                app.options.heightString[app.options.cursor2] = event->keysym.scancode - SDL_SCANCODE_1 + 1;
                app.options.cursor2++;
            }
        } else if(app.options.button3) {
            if(event->keysym.scancode == SDL_SCANCODE_BACKSPACE && app.options.cursor3 > 0){
                app.options.cursor3--;
            } else if(event->keysym.scancode == SDL_SCANCODE_0 && app.options.cursor3 < 3) {
                app.options.minesString[app.options.cursor3] = 0;
                app.options.cursor3++;
            } else if(event->keysym.scancode < SDL_SCANCODE_0 && event->keysym.scancode >= SDL_SCANCODE_1 && app.options.cursor3 < 3) {
                app.options.minesString[app.options.cursor3] = event->keysym.scancode - SDL_SCANCODE_1 + 1;
                app.options.cursor3++;
            }
        }
        
        if(event->keysym.scancode == SDL_SCANCODE_RETURN || event->keysym.scancode == SDL_SCANCODE_RETURN2 || event->keysym.scancode == SDL_SCANCODE_KP_ENTER) {
            int length = parseTextBox(app.options.cursor1, app.options.lengthString);
            int height = parseTextBox(app.options.cursor2, app.options.heightString);
            int mines = parseTextBox(app.options.cursor3, app.options.minesString);

            bool valid = true;

            if(length > 50) {
                app.options.cursor1 = 0;
                valid = false;
            }

            if(height > 20) {
                app.options.cursor2 = 0;
                valid = false;
            }

            if(app.options.cursor1 == 0 || app.options.cursor2 == 0 || app.options.cursor3 == 0) {
                valid = false;
            }

            if(valid) {
                app.info.menuOpen = false;
                app.options.customMenu = false;
                app.options.cursor1 = 0;
                app.options.cursor2 = 0;
                app.options.cursor3 = 0;

                newGame(app.game, length, height, mines);
                setNewWindowSize();
            }
        }

        *updateScreen = true;
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
                        if(!app.info.menuOpen) {
                            flag(app.game, app.info.mouseX, app.info.mouseY);
                        }
                        break;
                }
                updateScreen = true;
                oldX = -1;
                oldY = -1;
                break;
            case SDL_KEYUP:
                handleKeyboard(&event.key, &updateScreen);
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