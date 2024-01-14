#include <stdio.h>
#include "init.h"
#include "input.h"
#include "draw.h"
#include "maze.h"
#include "def.h"
#include <time.h>
/*
Compilation du programme :
gcc src/*.c -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
bin\prog.exe
*/

Bool checkCollision(Maze *maze, Entity *entity, int newX, int newY) {
    int cellX = newX / CELL_SIZE;
    int cellY = newY / CELL_SIZE;

    if (cellX >= 0 && cellX < maze->width && cellY >= 0 && cellY < maze->height) {
        if (maze->tab[cellX][cellY].isWall == TRUE) {
            return TRUE; 
        }
    }

    return FALSE; 
}

int main(int argc, char **argv) {
    App *app = initSDL();
    Maze *maze = malloc(sizeof(Maze));
    Entity *player = malloc(sizeof(Entity));

    if (app == NULL || maze == NULL || player == NULL) {
        SDL_ExitWithError("Allocation de mémoire initiale a échoué", app, maze, player);
    }

    appInit(app);

    srand(time(NULL)); 
    generateMaze(maze);
    player->texture = loadTexture("img/full.png", app);

    int characterSize = 25; 

    int startX = (SCREEN_WIDTH - (maze->width * CELL_SIZE)) / 2 + CELL_SIZE + (CELL_SIZE / 2) - (characterSize / 2);
    int startY = (SCREEN_HEIGHT - (maze->height * CELL_SIZE)) / 2 + CELL_SIZE + (CELL_SIZE / 2) - (characterSize / 2);
    player->x = startX;
    player->y = startY;

    // Variables pour stocker la position précédente du joueur
    int prevX = player->x;
    int prevY = player->y;

    SDL_Texture *buffer = SDL_CreateTexture(app->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    if(buffer == NULL) {
        SDL_ExitWithError("Erreur lors de la création du buffer", app, maze, player);
    }

    SDL_SetRenderTarget(app->renderer, buffer);
    renderMaze(app->renderer, maze, app);
    SDL_SetRenderTarget(app->renderer, NULL);

    while (app->programLaunched) {     
        inputEvent(app);

        // Copiez la position actuelle du joueur
        int newX = player->x;
        int newY = player->y;

        if (app->up && !app->down) {
            newY -= CELL_SIZE; 
        } else if (app->down && !app->up) {
            newY += CELL_SIZE;
        }

        if (app->left && !app->right) {
            newX -= CELL_SIZE; 
        } else if (app->right && !app->left) {
            newX += CELL_SIZE;
        }

        if (!checkCollision(maze, player, newX, newY)) {
            player->x = newX;
            player->y = newY;
        }

        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
        SDL_RenderClear(app->renderer);

        SDL_RenderCopy(app->renderer, buffer, NULL, NULL);

        blit(player->texture, player->x, player->y, app);

        presentScene(app);
        doInput(app);
    }

    SDL_Exit(app, maze, player);

    return EXIT_SUCCESS;
}



