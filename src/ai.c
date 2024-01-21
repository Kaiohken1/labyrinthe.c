#include "ai.h"
#include <stdio.h>

void loadAi(App *app, Entity *enemy, int startX, int startY) {
    enemy->texture = loadTexture("img/ai.jpg", app);
    enemy->x = startX;
    enemy->y = startY;
}

void updateAiPosition(Entity *enemy, Entity *player, Maze *maze) {
    static int moveTimer = 0;
    const int DELAY = 560;

    moveTimer++;
    if (moveTimer >= DELAY) {
        moveTimer = 0;

        int dx = 0, dy = 0;

        if (enemy->x < player->x) {
            dx = CELL_SIZE; 
        } else if (enemy->x > player->x) {
            dx = -CELL_SIZE;
        }

        if (enemy->y < player->y) {
            dy = CELL_SIZE; 
        } else if (enemy->y > player->y) {
            dy = -CELL_SIZE; 
        }

        if (rand() % 2 == 0) {
            dy = 0; 
        } else {
            dx = 0;
        }

        int aiX = enemy->x + dx;
        int aiY = enemy->y + dy;

        if (aiX >= 0 && aiX < MAZE_WIDTH * CELL_SIZE && aiY >= 0 && aiY < MAZE_HEIGHT * CELL_SIZE && !checkCollision(maze, enemy, aiX, aiY)) {
            enemy->x = aiX;
            enemy->y = aiY;
        }
    }
}






