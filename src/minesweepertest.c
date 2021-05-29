#include "unity.h"
#include "enums.h"
#include "minesweeper.h"

/* As this game is meant to be played, I can skip a few of the more complex tests as I'll notice them during gameplay
 * These tests are all the tiresome things I need to make sure are working after every change*/
void setUp(void) {}

void tearDown(void) {}

void createGameWorks(void) {
    Game* game = createGame(10, 20, 50);

    TEST_ASSERT_MESSAGE(getLength(game) == 10, "Length was not properly set");
    TEST_ASSERT_MESSAGE(getHeight(game) == 20, "Height was not properly set");
    TEST_ASSERT_MESSAGE(getMines(game) == 50, "Number of mines was not properly set");

    deleteGame(game);
}

void correctsLessThan0Size(void) {
    Game* badLength = createGame(-10, 10, 5);
    Game* badHeight = createGame(10, 0, 5);
    Game* badEverything = createGame(0, -5, 1);

    TEST_ASSERT_MESSAGE(getLength(badLength) == 1, "Length <= 0 not corrected");
    TEST_ASSERT_MESSAGE(getHeight(badHeight) == 1, "Height <= 0 not corrected");

    TEST_ASSERT_MESSAGE(getLength(badEverything) == 1, "Length <= 0 not corrected when height <= 0");
    TEST_ASSERT_MESSAGE(getHeight(badEverything) == 1, "Height <= 0 not corrected when length <= 0");

    deleteGame(badLength);
    deleteGame(badHeight);
    deleteGame(badEverything);
}

void correctsInvalidNumberOfMines(void) {
    Game* lessThan0Mines = createGame(5, 5, -19);
    Game* moreThanMaxMines = createGame(5, 5, 100);

    TEST_ASSERT_MESSAGE(getMines(lessThan0Mines) == 0, "Creating less than 0 mines does not correct to 0");
    TEST_ASSERT_MESSAGE(getMines(moreThanMaxMines) == 25, "Creating more than max mines does not correct to max");

    deleteGame(lessThan0Mines);
    deleteGame(moreThanMaxMines);
}

void revealReturnsActive(void) {
    Game* game = createGame(10, 10, 75);

    TEST_ASSERT(reveal(game, 5, 5) == ACTIVE);

    deleteGame(game);
}

void revealReturnsLoss(void) {
    Game* game = createGame(10, 10, 100);

    TEST_ASSERT(reveal(game, 5, 5) == LOSS);

    deleteGame(game);
}

void firstRevealIsNotAMine(void) {
    Game* game = createGame(10, 10, 99);

    enum GameState gameState = reveal(game, 5, 5);

    enum Tile** board = getBoard(game);

    TEST_ASSERT_MESSAGE(gameState == WIN, "Game was not won");

    TEST_ASSERT(board[5][5] == REVEALED_8);

    deleteGame(game);
}

void surroundingRevealAreNotMines(void) {
    Game* game = createGame(10, 10, 91);

    enum GameState gameState = reveal(game, 5, 5);

    enum Tile** board = getBoard(game);

    TEST_ASSERT_MESSAGE(gameState == WIN, "Game was not won");

    TEST_ASSERT(board[4][4] == REVEALED_5);
    TEST_ASSERT(board[5][4] == REVEALED_3);
    TEST_ASSERT(board[6][4] == REVEALED_5);
    TEST_ASSERT(board[4][5] == REVEALED_3);
    TEST_ASSERT(board[5][5] == REVEALED_0);
    TEST_ASSERT(board[6][5] == REVEALED_3);
    TEST_ASSERT(board[4][6] == REVEALED_5);
    TEST_ASSERT(board[5][6] == REVEALED_3);
    TEST_ASSERT(board[6][6] == REVEALED_5);

    deleteGame(game);
}

void surroundingRevealAreNotMinesOnWall(void) {
    Game* game = createGame(10, 10, 94);

    enum GameState gameState = reveal(game, 9, 5);

    enum Tile** board = getBoard(game);

    TEST_ASSERT(gameState == WIN);

    TEST_ASSERT(board[8][4] == REVEALED_5);
    TEST_ASSERT(board[9][4] == REVEALED_2);
    TEST_ASSERT(board[8][5] == REVEALED_3);
    TEST_ASSERT(board[9][5] == REVEALED_0);
    TEST_ASSERT(board[8][6] == REVEALED_5);
    TEST_ASSERT(board[9][6] == REVEALED_2);

    deleteGame(game);
}

void surroundingRevealAreNotMinesOnCorner(void) {
    Game* game = createGame(10, 10, 96);

    enum GameState gameState = reveal(game, 9, 0);

    enum Tile** board = getBoard(game);

    TEST_ASSERT(gameState == WIN);

    TEST_ASSERT(board[8][0] == REVEALED_2);
    TEST_ASSERT(board[9][0] == REVEALED_0);
    TEST_ASSERT(board[8][1] == REVEALED_5);
    TEST_ASSERT(board[9][1] == REVEALED_2);

    deleteGame(game);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(createGameWorks);
    RUN_TEST(correctsLessThan0Size);
    RUN_TEST(correctsInvalidNumberOfMines);
    RUN_TEST(revealReturnsActive);
    RUN_TEST(revealReturnsLoss);
    RUN_TEST(firstRevealIsNotAMine);
    RUN_TEST(surroundingRevealAreNotMines);
    RUN_TEST(surroundingRevealAreNotMinesOnWall);
    RUN_TEST(surroundingRevealAreNotMinesOnCorner);

    return UNITY_END();
}