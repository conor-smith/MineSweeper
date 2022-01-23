#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "structs.h"
#include "minesweeper.h"
#include "defs.h"
#include "window.h"

App app;

SDL_Texture *createText(TTF_Font *font, const char *text, SDL_Color colour) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, colour);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer, surface);
}

// Just a big ol' grand initialization of everything
void init(void) {
    app.game = createGame(16, 16, 40);

    //Set up SDL window and renderer
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    app.window = SDL_CreateWindow(
        "MineSweeper", // Window title
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, // Window position x and y. Undefined so window manager decides
        500, 500, // Window height and width
        0); // Flags. None are needed

    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Set window icon
    SDL_Surface *icon = IMG_Load("img/icon.png");

    SDL_SetWindowIcon(app.window, icon);

    // Load textures
    IMG_Init(IMG_INIT_PNG);

    app.info.texture = IMG_LoadTexture(app.renderer, "img/textures.png");
    app.info.analogue = IMG_LoadTexture(app.renderer, "img/analogue.png");
    app.info.face = IMG_LoadTexture(app.renderer, "img/face.png");

    // Make textures for text
    if(TTF_Init() != 0) {
        printf("couldn't initialize text: %s\n", SDL_GetError());
        exit(1);
    }

    TTF_Font *sans = TTF_OpenFont("img/OpenSans-Bold.ttf", 20);
    SDL_Color black = {0, 0, 0};
    SDL_Color white = {255, 255, 255};
    SDL_Color grey = {127, 127, 127};

    app.options.gameb = createText(sans, "Game", black);
    app.options.gamew = createText(sans, "Game", white);
    app.options.beginner = createText(sans, "Beginner", black);
    app.options.beginnerSpec = createText(sans, "9x9, 10 mines", black);
    app.options.intermediate = createText(sans, "Intermediate", black);
    app.options.intermediateSpec = createText(sans, "16x16, 40 mines", black);
    app.options.expert = createText(sans, "Expert", black);
    app.options.expertSpec = createText(sans, "16x30, 99 mines", black);
    app.options.custom = createText(sans, "Custom", black);
    app.options.length = createText(sans, "length", black);
    app.options.height = createText(sans, "height", black);
    app.options.mines = createText(sans, "mines", black);
    app.options.lengthWarning = createText(sans, "<=50", grey);
    app.options.lengthWarning = createText(sans, "<=20", grey);
    app.options.lengthWarning = createText(sans, "<=999", grey);
    app.options.cursor1 = 0;
    app.options.cursor2 = 0;
    app.options.cursor3 = 0;
    app.options.customMenu = false;

    char numberString[2];
    numberString[1] = '\0';

    for(char i = '0';i <= '9';i++) {
        numberString[0] = i;
        app.options.numbers[i - '0'] = createText(sans, numberString, black);
    }

    TTF_CloseFont(sans);

    // Set up other data
    SDL_QueryTexture(app.options.intermediate, NULL, NULL, &app.options.buttonWidth, &app.options.textHeight);
    app.options.buttonWidth += MENU_PADDING * 2;

    int throwAway;
    SDL_QueryTexture(app.options.length, NULL, NULL, &app.options.labelWidth, &throwAway);
    SDL_QueryTexture(app.options.gameb, NULL, NULL, &app.options.gameButtonWidth, &throwAway);

    int infoWidth;
    SDL_QueryTexture(app.options.expertSpec, NULL, NULL, &infoWidth, &app.options.textHeight);

    app.options.boxWidth = MENU_PADDING * 3 + app.options.buttonWidth + infoWidth;
    app.options.boxHeight = app.options.textHeight * 4 + MENU_PADDING * 2;
    app.options.textBoxWitdh = app.options.boxWidth - (MENU_PADDING * 3 + app.options.labelWidth);

    app.options.button1y = app.options.textHeight + MENU_PADDING;
    app.options.button2y = app.options.textHeight * 2 + MENU_PADDING;
    app.options.button3y = app.options.textHeight * 3 + MENU_PADDING;
    app.options.button4y = app.options.textHeight * 4 + MENU_PADDING;

    setNewWindowSize();
}

// While not technically necessary, still safer and ultimately helps catch errors
void cleanup(void) {
    SDL_DestroyRenderer(app.renderer);

    SDL_DestroyWindow(app.window);

    SDL_DestroyTexture(app.info.texture);
    SDL_DestroyTexture(app.info.analogue);

    deleteGame(app.game);
}