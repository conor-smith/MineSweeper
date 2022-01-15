#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include "../enums.h"
#include "../minesweeper.h"

// A couple of colour constants
#define YELLOW  1
#define RED     2
#define GREEN   3
#define CYAN    4

// Arrays to store characters to print
chtype hidden = '*';
chtype mine[3];
chtype flagHidden[3];
chtype flagMine[3];

typedef struct {
    int x,y;
} GameCursor;

// Doesn't allow numbers above 99
int getNumber(char *string) {
    addstr(string);

    int cursor = 0;
    char input = 0;
    char inputNumber[] = {0,0};
    // 10 = value of enter key
    while(input != 10) {
        input = getch();

        if(input >= '0' && input <= '9' && cursor < 2) {
            // Mimics echo(), but only for numeric characters
            addch(input);
            inputNumber[cursor] = input;
            cursor++;
        // 127 = numeric value of backspace. This deletes a character
        } else if(input == 127 && cursor > 0) {
            int x, y;
            getyx(stdscr, y, x);
            move(y, x-1);
            delch();
            inputNumber[cursor] = 0;
            cursor--;
        }
    }
    addch('\n');

    switch(cursor) {
        case 1:
            return inputNumber[0] - '0';
        case 2:
            return (inputNumber[0] - '0') * 10 + (inputNumber[1] - '0');
        default:
            //Just here to make the compiler happy
            return 0;
    }
}

void setDifficulty(Game *game) {
    // A struct to keep track of inputs
    struct Inputs {
        int length, height, mines;
    };

    erase();

    // Offset is the line on which the first option is displayed
    const int offset = 3;
    const int noOfOptions = 4;

    // An array containing all the options. The last value is always custom
    char *options[] = {
        "Beginner         9       9       10\n",
        "Intermediate     16      16      40\n",
        "Expert           30      16      99\n",
        "Custom"};

    struct Inputs inputOptions[] = {
        {9, 9, 10},
        {16, 16, 40},
        {30, 16, 99}};

    addstr("Select your difficulty\n\n");
    addstr("                 Length  Height  Mines\n");

    // Print all options
    for(int i = 0;i < noOfOptions;i++) {
        addstr(options[i]);
    }

    refresh();

    // Allow user to select option
    int input = 0;
    int selectedOption = 0;
    for(;input != 10;) {
        attron(A_STANDOUT);
        move(selectedOption+offset, 0);
        addstr(options[selectedOption]);

        input = getch();

        attroff(A_STANDOUT);
        move(selectedOption+offset, 0);

        if(input == KEY_UP && selectedOption > 0) {
            addstr(options[selectedOption]);
            selectedOption--;
        } else if(input == KEY_DOWN && selectedOption < noOfOptions-1) {
            addstr(options[selectedOption]);
            selectedOption++;
        }
    }

    if(selectedOption < noOfOptions-1) {
        struct Inputs *inputs = inputOptions + selectedOption;
        newGame(game, inputs->length, inputs->height, inputs->mines);
    } else {
        move(offset+noOfOptions+1, 0);
        curs_set(1);
        int length = getNumber("Length: ");
        int height = getNumber("Height: ");
        int mines = getNumber("Mines: ");
        erase();
        newGame(game, length, height, mines);
        curs_set(0);
    }
}

chtype getCharacter(tile value, state gameState) {
    /* Follow a different set of instructions whether or not the 
             * value is hidden, flagged or revealed*/
            switch(value / FLAGGED_0) {
                case 0:
                    if(value == HIDDEN_MINE) {
                        return mine[gameState];
                    } else {
                        return hidden;
                    }
                    break;
                case 1:
                    if(value == FLAGGED_MINE) {
                        return flagMine[gameState];
                    } else {
                        return flagHidden[gameState];
                    }
                    break;
                case 2:
                    switch(value) {
                        case REVEALED_MINE:
                            return mine[gameState];
                            break;
                        case REVEALED_0:
                            return ' ';
                            break;
                        default:
                            return ((char)value-REVEALED_0+'0') | COLOR_PAIR(YELLOW);
                            break;
                    }
                default:
                    // Just here so the compiler doesn't complain
                    return 'y';
            }
}

void renderBoard(Game *game) {
    // Render board
    move(0, 0);
    tile **board = getBoard(game);
    for(int y = 0;y < getHeight(game);y++) {
        for(int x = 0;x < getLength(game);x++) {
            addch(getCharacter(board[x][y], getGameState(game)));
            // To better space out the characters
            addch(' ');
        }
        // Render next line
        addch('\n');
    }

    refresh();
}

void renderGameCursor(Game *game, GameCursor gameCursor, bool *cursorDidNotMove) {
    static int previousX, previousY;

    if(*cursorDidNotMove) {
        *cursorDidNotMove = false;
    } else {
        chtype previousCharacter = getCharacter(getBoard(game)[previousX][previousY], getGameState(game));
        mvaddch(previousY, previousX*2, previousCharacter);
    }

    chtype cursor = mvinch(gameCursor.y, gameCursor.x*2) | A_STANDOUT;
    mvaddch(gameCursor.y, gameCursor.x*2, cursor);

    previousX = gameCursor.x;
    previousY = gameCursor.y;
}

// Returns true if y, false if n
bool getYOrN(int yLocation, char* question) {
    move(yLocation, 0);
    while(true) {
        addstr(question);
        refresh();
        switch(getch()) {
            case 'y':
                move(yLocation, 0);
                clrtoeol();
                move(yLocation+1, 0);
                clrtoeol();
                return true;
            case 'n':
                move(yLocation, 0);
                clrtoeol();
                move(yLocation+1, 0);
                clrtoeol();
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
    // Used to render cursor for first move
    bool cursorDidNotMove = true;

    // Game loop. Can only be exited through quitting the game
    while(true) {
        beginGame:
        // Initialize game
        setDifficulty(game);
        GameCursor gameCursor = {0, 0};

        // Render the board
        renderBoard(game);
        while(getGameState(game) == ACTIVE) {
            // Play game
            renderGameCursor(game, gameCursor, &cursorDidNotMove);

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
                    reveal(game, gameCursor.x, gameCursor.y);
                    renderBoard(game);
                    cursorDidNotMove = true;
                    break;
                case 'f':
                    flag(game, gameCursor.x, gameCursor.y);
                    renderBoard(game);
                    cursorDidNotMove = true;
                    break;
                case 'v':
                    if(getYOrN(getHeight(game)+2, "Are you sure you want to reset?(y/n)")) {
                        goto beginGame;
                    }
                    break;
                case 'q':
                    if(getYOrN(getHeight(game)+2, "Are you sure you want to quit?(y/n)")) {
                        return;
                    }
                    break;
            }
        }

        // End game
        move(getHeight(game)+2, 0);
        switch(getGameState(game)) {
            case WIN:
                addstr("CONGRATRULATIONS. YOU WIN");
                break;
            case LOSS:
                addstr("GAME OVER");
                break;
            default:
                addstr("Somethign bad has happened");
                break;
        }
        if(!getYOrN(getHeight(game)+3, "Play again?(y/n)")) {
            return;
        }
    }

    /* Free resources
     * Not technically necessary as game as program should end after, but best be safe
     * in case I modify something and forget*/
    deleteGame(game);
}

int main(void) {
    /* Initialize ncurses
     * Remove echo, use colour and default colours,
     * Ensure input is processed immediately*/
    initscr();
    noecho();
    cbreak();
    start_color();
    use_default_colors();
    
    // Make cursor invisible and allow arrow key inputs
    curs_set(0);
    keypad(stdscr, true);

    // Set up colours
    init_pair(YELLOW, COLOR_YELLOW, -1);
    init_pair(RED, COLOR_RED, -1);
    init_pair(GREEN, COLOR_GREEN, -1);
    init_pair(CYAN, COLOR_CYAN, -1);

    /* Set up characters
     * LOSS = 0, ACTIVE = 1, WIN = 2*/
    mine[LOSS] = 'X' | COLOR_PAIR(RED) | A_BOLD;
    mine[ACTIVE] = hidden;
    mine[WIN] = 'F' | COLOR_PAIR(GREEN);
    flagHidden[LOSS] = 'F' | COLOR_PAIR(RED);
    flagHidden[ACTIVE] = 'F' | COLOR_PAIR(CYAN);
    flagMine[LOSS] = flagHidden[ACTIVE];
    flagMine[ACTIVE] = flagHidden[ACTIVE];
    flagMine[WIN] = mine[WIN];

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
    getch();

    gameLoop(createEmptyGame());

    erase();
    endwin();

    printf("Thank you for playing!\n");
}