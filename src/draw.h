#ifndef __DRAW__
#define __DRAW__

void prepareScene(App *app);
void presentScene(App *app);
SDL_Texture *loadTexture(char *filename, App *app);
void showTexture(SDL_Texture *texture, int x, int y, int width, int height, App *app);
void blit(SDL_Texture *texture, int x, int y, App *app);
void drawText(App *app, const char *message);

void doInput(App *app);
#endif