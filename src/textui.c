#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include "enums.h"
#include "minesweeper.h"

struct Inputs {
    int length, height, mines;
};

int getNumber(char *string) {
    char inputString[5];

    addstr(string);
    refresh();

    scanw(inputString);

    return atoi(inputString);
}

void selectDifficulty(struct Inputs *inputs) {
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
        *inputs = inputOptions[selectedOption];
    } else {
        move(offset+noOfOptions+1, 0);
        echo();
        curs_set(1);
        keypad(stdscr, false);
        inputs->length = getNumber("Length: ");
        inputs->height = getNumber("Height: ");
        inputs->mines = getNumber("Mines: ");
        noecho();
        curs_set(0);
        keypad(stdscr, true);
    }
}

void gameLoop() {
    struct Inputs inputs;
    selectDifficulty(&inputs);

    erase();
    move(0,0);

    printw("Inputs\nLength:%d\nHeight:%d\nMines:%d", inputs.length, inputs.height, inputs.mines);

    getch();
}

void printGreetings() {
    addstr("-----------------------\n");
    addstr("      MINESWEEPER      \n");
    addstr("-----------------------\n\n");

    refresh();
}

int main(int argc, char** argv) {
    // Initialize ncurses
    initscr();
    curs_set(0);
    noecho();
    start_color();
    keypad(stdscr, true);

    printGreetings();

    addstr("Press any key to continue\n");
    getch();

    gameLoop();
    getch();

    endwin();
}