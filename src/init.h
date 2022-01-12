#ifndef INIT_H
#define INIT_H

#include <SDL2/SDL_render.h>

void init(void);
void cleanup(void);
SDL_Texture *loadTexture(char *filename);

#endif