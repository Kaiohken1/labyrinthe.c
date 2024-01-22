#include <stdio.h>
#include "init.h"
#include "input.h"
#include "draw.h"
#include "maze.h"
#include "def.h"
#include "level.h"
#include <time.h>
#include "ini.h"
#include <string.h>
#include "config.h"


/*
Compilation du programme :
gcc src/*.c -o bin/monjeu -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

bin\prog.exe
*/








int main(int argc, char **argv) {
  int NumberOfLevels = parseIniFile("src/config.onoo");
     printf("Nombre de niveaux : %d\n", NumberOfLevels);
    App *app = initSDL();

    if (app == NULL) {
        SDL_ExitWithError("Initialisation SDL a échoué", NULL, NULL, NULL);
    }

    srand(time(NULL)); 
    int level = 1;
    List timeList = newList();
    time_t startTime, endTime;

    while (level <= NumberOfLevels) {
        Maze *maze = malloc(sizeof(Maze));
        Entity *player = malloc(sizeof(Entity));

        if (maze == NULL || player == NULL) {
            SDL_ExitWithError("Allocation de mémoire pour le niveau a échoué", app, maze, player);
        }

        appInit(app);

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

        Bool levelCompleted = FALSE;
        clock_t startClock = clock();

        while (app->programLaunched) {    
            inputEvent(app);

    
            int newX = player->x;
            int newY = player->y;
            const int delay = 100; 
            static int lastMoveTime = 0;
            int currentTime = SDL_GetTicks();
            
            if (currentTime - lastMoveTime > delay) {
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
   lastMoveTime = currentTime;
}
            if (!checkCollision(maze, player, newX, newY)) {
                player->x = newX;
                player->y = newY;
            }

            prepareScene(app);

            SDL_RenderCopy(app->renderer, buffer, NULL, NULL);

            blit(player->texture, player->x, player->y, app);

            presentScene(app);
            doInput(app);


            if (player->x < 0 || player->x >= SCREEN_WIDTH || player->y < 0 || player->y >= SCREEN_HEIGHT) {
                levelCompleted = TRUE; 
            }

            if (levelCompleted) {
                clock_t endClock = clock();
                double elapsedTime = (double)(endClock - startClock) / CLOCKS_PER_SEC;
                timeList = pushBackList(timeList, elapsedTime);
                level++; 
                break;
            }
        }

        freeGrid(maze);
        free(maze);
        free(player);
    }

    double total = getTotalTime(timeList);
    char timeStr[100];

    sprintf(timeStr,"Vous vous etes echappes en : %.2lf secondes", total);

    drawText(app, timeStr); 

    while (app->programLaunched) {
        inputEvent(app);
    }


    timeList = clearList(timeList);
    SDL_Exit(app, NULL, NULL);

    return EXIT_SUCCESS;
}
