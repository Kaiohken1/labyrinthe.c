#ifndef __DRAW__
#define __DRAW__
#include <SDL_ttf.h>

void prepareScene(App *app);
void presentScene(App *app);
SDL_Texture *loadTexture(char *filename, App *app);
void showTexture(SDL_Texture *texture, int x, int y, int width, int height, App *app);
void blit(SDL_Texture *texture, int x, int y, App *app);
void drawText(App *app, const char *message, int x, int y);
TTF_Font* loadFont(char *filename, int fontSize);

void doInput(App *app);
#endif