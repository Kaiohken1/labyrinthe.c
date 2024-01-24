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
#include "database.h"
#include "ai.h"

/*
Compilation du programme :
gcc src/*.c -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
bin\prog.exe
*/

void runGame(App *app) {
    int level = 1;
    List timeList = newList();
    Bool gameOver = FALSE;
    time_t startTime, endTime;
    int NumberOfLevels = parseIniFileint("NumberOfLevels", app);
    app->ai = parseIniFileString("AI", app);
    Bool isEnabled = FALSE;
    if(strcmp(app->ai, "true") == 0) {
        isEnabled = TRUE;
    }

    if(NumberOfLevels < 1) {
        SDL_ExitWithError("Aucun niveau chargé, veuillez initialiser la valeur à 1 minimum", app, NULL, NULL, NULL);
    } else if(NumberOfLevels > 6) {
        NumberOfLevels = 5;
    }

    while (level <= NumberOfLevels && gameOver == FALSE) {
        Maze *maze = malloc(sizeof(Maze));
        Entity *player = malloc(sizeof(Entity));
        Entity *ai = malloc(sizeof(Entity));

        if (maze == NULL || player == NULL || ai == NULL) {
            SDL_ExitWithError("Allocation de mémoire pour le niveau a échoué", app, maze, player, ai);
        }

        appInit(app);

        generateMaze(maze, app);
        player->texture = loadTexture("img/full.png", app);

        int characterSize = 25; 

        int startX = (app->screenWidth - (maze->width * CELL_SIZE)) / 2 + CELL_SIZE + (CELL_SIZE / 2) - (characterSize / 2);
        int startY = (app->screenHeight - (maze->height * CELL_SIZE)) / 2 + CELL_SIZE + (CELL_SIZE / 2) - (characterSize / 2);
        int endX = (app->screenWidth - (maze->width * CELL_SIZE)) / 2 + (maze->width - 1) * CELL_SIZE + (CELL_SIZE / 2) - (characterSize / 2);
        int endY = (app->screenHeight - (maze->height * CELL_SIZE)) / 2 + (maze->height - 2) * CELL_SIZE + (CELL_SIZE / 2) - (characterSize / 2);

        player->x = startX;
        player->y = startY;

        int prevX = player->x;
        int prevY = player->y;
        if(isEnabled) {
            loadAi(app, ai, endX, endY);
        }

        SDL_Texture *buffer = SDL_CreateTexture(app->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, app->screenWidth, app->screenHeight);
        if(buffer == NULL) {
            SDL_ExitWithError("Erreur lors de la création du buffer", app, maze, player, ai);
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
            const int delay = 70; 
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

            if (player->x == ai->x && player->y == ai->y && isEnabled) {
                gameOver = TRUE;
                break;
            }

            if(isEnabled) {
                updateAiPosition(ai, player, maze);
            }

            prepareScene(app);

            SDL_RenderCopy(app->renderer, buffer, NULL, NULL);
            blit(player->texture, player->x, player->y, app);
            
            if(isEnabled) {
                blit(ai->texture, ai->x, ai->y, app);
            }

            presentScene(app);
            doInput(app);


            if (player->x < 0 || player->x > maze->width * CELL_SIZE|| player->y < 0 || player->y > maze->height * CELL_SIZE) {
                levelCompleted = TRUE; 
            }

            if (levelCompleted && gameOver == FALSE) {
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

    if(gameOver) {
        prepareScene(app);
        drawText(app, "Vous avez perdu !", 100, 50);
        drawText(app, "Appuyez sur R pour rejouer", 100, 150); 
        presentScene(app);
        while (app->programLaunched) {
            inputEvent(app);
            if (app->restart) {
                app->restart = SDL_FALSE;
                runGame(app);
            break;
            }
        }
    } else {
        double total = getTotalTime(timeList);
        char timeStr[100];
        sprintf(timeStr,"Vous vous etes echappes en : %.2lf secondes", total);
        char playerName[10] = {0};
        getUserInput(app, timeStr, playerName, 10);

        sqlite3 *db;
        callDB(db, app, total, playerName);

        while (app->programLaunched) {
            inputEvent(app);
            if (app->restart) {
                app->restart = SDL_FALSE;
                runGame(app);
            break;
            }
        }

        timeList = clearList(timeList);
    }
}

int main(int argc, char **argv) {
    App *app = initSDL();

    if (app == NULL) {
        SDL_ExitWithError("Initialisation SDL a échoué", NULL, NULL, NULL, NULL);
    }

    srand(time(NULL)); 
    
    runGame(app);

    SDL_Exit(app, NULL, NULL, NULL);

    return EXIT_SUCCESS;
}
