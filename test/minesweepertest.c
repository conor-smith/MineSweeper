#include "unity.h"
#include "enums.h"
#include "minesweeper.h"

/* As this game is meant to be played, I can skip a few of the more complex tests as I'll notice them during gameplay
 * These tests are all the tiresome things I need to make sure are working after every change*/
void setUp(void) {}

void tearDown(void) {}

void test_createGameWorks(void) {
    Game* game = createGame(10, 20, 50);

    TEST_ASSERT_MESSAGE(getLength(game) == 10, "Length was not properly set");
    TEST_ASSERT_MESSAGE(getHeight(game) == 20, "Height was not properly set");
    TEST_ASSERT_MESSAGE(getMines(game) == 50, "Number of mines was not properly set");

    deleteGame(game);
}

void test_correctsLessThan0Size(void) {
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

void test_correctsInvalidNumberOfMines(void) {
    Game* lessThan0Mines = createGame(5, 5, -19);
    Game* moreThanMaxMines = createGame(5, 5, 100);

    TEST_ASSERT_MESSAGE(getMines(lessThan0Mines) == 0, "Creating less than 0 mines does not correct to 0");
    TEST_ASSERT_MESSAGE(getMines(moreThanMaxMines) == 25, "Creating more than max mines does not correct to max");

    deleteGame(lessThan0Mines);
    deleteGame(moreThanMaxMines);
}

void test_revealReturnsActive(void) {
    Game* game = createGame(10, 10, 75);

    TEST_ASSERT(reveal(game, 5, 5) == ACTIVE);

    deleteGame(game);
}

void test_revealReturnsLoss(void) {
    Game* game = createGame(10, 10, 100);

    TEST_ASSERT(reveal(game, 5, 5) == LOSS);

    deleteGame(game);
}

void test_firstRevealIsNotAMine(void) {
    Game* game = createGame(10, 10, 99);

    state gameState = reveal(game, 5, 5);

    tile** board = getBoard(game);

    TEST_ASSERT_MESSAGE(gameState == WIN, "Game was not won");

    TEST_ASSERT(board[5][5] == REVEALED_8);

    deleteGame(game);
}

void test_surroundingRevealAreNotMines(void) {
    Game* game = createGame(10, 10, 91);

    state gameState = reveal(game, 5, 5);

    tile** board = getBoard(game);

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

void test_surroundingRevealAreNotMinesOnWall(void) {
    Game* game = createGame(10, 10, 94);

    state gameState = reveal(game, 9, 5);

    tile** board = getBoard(game);

    TEST_ASSERT(gameState == WIN);

    TEST_ASSERT(board[8][4] == REVEALED_5);
    TEST_ASSERT(board[9][4] == REVEALED_2);
    TEST_ASSERT(board[8][5] == REVEALED_3);
    TEST_ASSERT(board[9][5] == REVEALED_0);
    TEST_ASSERT(board[8][6] == REVEALED_5);
    TEST_ASSERT(board[9][6] == REVEALED_2);

    deleteGame(game);
}

void test_surroundingRevealAreNotMinesOnCorner(void) {
    Game* game = createGame(10, 10, 96);

    state gameState = reveal(game, 9, 0);

    tile** board = getBoard(game);

    TEST_ASSERT(gameState == WIN);

    TEST_ASSERT(board[8][0] == REVEALED_2);
    TEST_ASSERT(board[9][0] == REVEALED_0);
    TEST_ASSERT(board[8][1] == REVEALED_5);
    TEST_ASSERT(board[9][1] == REVEALED_2);

    deleteGame(game);
}

void test_canFlagTilesBeforeGame(void) {
    Game* game = createGame(10, 10, 0);

    flag(game, 5, 5);

    state gameState = reveal(game, 0, 0);

    tile** board = getBoard(game);

    TEST_ASSERT(gameState == ACTIVE);
    TEST_ASSERT(board[5][5] == FLAGGED_0);

    deleteGame(game);
}

void test_gameCanBeReset(void) {
    Game* game = createGame(5, 5, 16);

    reveal(game, 2, 2);
    flag(game, 0, 0);

    tile** board = getBoard(game);

    TEST_ASSERT(board[2][2] == REVEALED_0);

    resetGame(game);

    TEST_ASSERT(board[2][2] == HIDDEN_0);
    TEST_ASSERT(board[0][0] == HIDDEN_MINE);
    TEST_ASSERT(board[4][4] == HIDDEN_MINE);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_createGameWorks);
    RUN_TEST(test_correctsLessThan0Size);
    RUN_TEST(test_correctsInvalidNumberOfMines);
    RUN_TEST(test_revealReturnsActive);
    RUN_TEST(test_revealReturnsLoss);
    RUN_TEST(test_firstRevealIsNotAMine);
    RUN_TEST(test_surroundingRevealAreNotMines);
    RUN_TEST(test_surroundingRevealAreNotMinesOnWall);
    RUN_TEST(test_surroundingRevealAreNotMinesOnCorner);
    RUN_TEST(test_canFlagTilesBeforeGame);
    RUN_TEST(test_gameCanBeReset);

    return UNITY_END();
}