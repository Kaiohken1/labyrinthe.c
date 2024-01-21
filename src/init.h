#ifndef __INIT__
#define __INIT__

#include "structs.h"

App *initSDL();
void SDL_ExitWithError(const char *message, App *app, Maze *maze, Entity *player, Entity *enemy);
void SDL_Exit(App *app, Maze * maze, Entity *player, Entity *enemy);
void appInit(App *app);

#endif