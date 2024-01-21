#ifndef __AI__
#define __AI__
#include "structs.h"
#include "def.h"
#include "maze.h"
#include "draw.h"

void loadAi(App *app, Entity *enemy, int startX, int startY);
void updateAiPosition(Entity *enemy, Entity *player, Maze *maze);

#endif