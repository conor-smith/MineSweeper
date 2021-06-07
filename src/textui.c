#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include "enums.h"
#include "minesweeper.h"

typedef struct {
    int x,y;
} GameCursor;

void setDifficulty(Game *game) {
    //TODO
}

void renderBoard(Game *game) {
    //TODO
}

void renderGameCursor(GameCursor gameCursor) {
    //TODO
}

// Returns true if y, false if n
bool getYOrN(int yLocation, char* question) {
    move(yLocation, 0);
    echo();
    curs_set(1);
    while(true) {
        addstr(question);
        refresh();
        switch(getch()) {
            case 'y':
                noecho();
                curs_set(0);
                return true;
            case 'n':
                noecho();
                curs_set(0);
                return false;
            default:
                move(yLocation+1, 0);
                addstr("Please enter y or n");
                move(yLocation, 0);
                clrtoeol();
                break;
        }
    }
}

/* The only thing handled in this function are collecting inputs
 * All logic and rendering is handled by other functions*/
void gameLoop(Game *game) {
    // Game loop. Can only be exited through quitting the game
    while(true) {
        // Initialize game
        beginGame:
        setDifficulty(game);
        GameCursor gameCursor = {0, 0};

        // Render the board
        renderBoard(game);
        while(getGameState(game) == ACTIVE) {
            // Play game
            renderGameCursor(gameCursor);

            switch(getch()) {
                case KEY_UP:
                    gameCursor.y = gameCursor.y > 0 ? gameCursor.y-1 : gameCursor.y;
                    break;
                case KEY_DOWN:
                    gameCursor.y = gameCursor.y < getHeight(game)-1 ? gameCursor.y+1 : gameCursor.y;
                    break;
                case KEY_LEFT:
                    gameCursor.x = gameCursor.x > 0 ? gameCursor.x-1 : gameCursor.x;
                    break;
                case KEY_RIGHT:
                    gameCursor.x = gameCursor.x < getLength(game)-1 ? gameCursor.x+1 : gameCursor.x;
                    break;
                case 'r':
                    //TODO
                    renderBoard(game);
                    break;
                case 'f':
                    //TODO
                    renderBoard(game);
                    break;
                case 'v':
                    switch(getYOrN(getHeight(game)+2, "Are you sure you want to reset?(y/n) ")) {
                        case true:
                            goto beginGame;
                    }
                    break;
                case 'q':
                    switch(getYOrN(getHeight(game)+2, "Are you sure you want to quit?(y/n) ")) {
                        case true:
                            return;
                    }
                    break;
            }
        }
    }
}

int main(int argc, char** argv) {
    /* Initialize ncurses
     * Remove echo, use colour and default colours,
     * Ensure input is processed immediately*/
    initscr();
    noecho();
    cbreak();
    start_color();
    use_default_colors();
    
    // Make cursor invisible and allow non ASCII key inputs
    curs_set(0);
    keypad(stdscr, true);

    // Print greeting
    addstr("-------------------\n");
    addstr("    MINESWEEPER    \n");
    addstr("-------------------\n\n");
    addstr("Arrow keys to move\n");
    addstr("r to reveal\n");
    addstr("f to flag\n");
    addstr("v to reset and q to quit\n");
    addstr("Press any key to continue");
    refresh();

    gameLoop(createEmptyGame());

    erase();
    endwin();

    printf("Thank you for playing!");
}