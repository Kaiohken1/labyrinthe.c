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

    if(NumberOfLevels < 1) {
        SDL_ExitWithError("Aucun niveau chargé, veuillez initialiser la valeur à 1 minimum", app, NULL, NULL, NULL);
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

        loadAi(app, ai, endX, endY);

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

            if (player->x == ai->x && player->y == ai->y) {
                gameOver = TRUE;
                break;
            }

            updateAiPosition(ai, player, maze);
            prepareScene(app);

            SDL_RenderCopy(app->renderer, buffer, NULL, NULL);
            blit(player->texture, player->x, player->y, app);
            blit(ai->texture, ai->x, ai->y, app);

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


void drawButton(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect rect, SDL_Color bgColor, SDL_Color textColor) {
    // Définir la couleur de fond du bouton
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    
    // Dessiner le rectangle du bouton
    SDL_RenderFillRect(renderer, &rect);

    // Calculez les coordonnées x et y pour centrer le texte dans le bouton
    int textWidth, textHeight;
    TTF_SizeText(font, text, &textWidth, &textHeight);
    int textX = rect.x + (rect.w - textWidth) / 2;
    int textY = rect.y + (rect.h - textHeight) / 2;

    // Dessiner le texte du bouton
    drawText(renderer, font, text, textX, textY, textColor);
}



int isMouseOverButton(int mouseX, int mouseY, SDL_Rect *rect) {

    // Code pour vérifier si la souris est sur le bouton
    return (mouseX > rect->x && mouseX < rect->x + rect->w &&
            mouseY > rect->y && mouseY < rect->y + rect->h);
}


void showMainMenu(App *app) {
    int menuRunning = 1;
    SDL_Rect startButton = {100, 50, 200, 50}; // x, y, width, height
    SDL_Rect optionsButton = {100, 150, 200, 50};
    SDL_Rect quitButton = {100, 250, 200, 50};

    while (menuRunning) {
        prepareScene(app);

        // Dessiner les boutons ici
        drawButton(app, "Démarrer le jeu", &startButton);
        drawButton(app, "Options", &optionsButton);
        drawButton(app, "Quitter", &quitButton);

        presentScene(app);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    menuRunning = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int mouseX = event.button.x;
                        int mouseY = event.button.y;
                        if (isMouseOverButton(mouseX, mouseY, &startButton)) {
                            runGame(app);
                        } else if (isMouseOverButton(mouseX, mouseY, &optionsButton)) {
                            // Options
                        } else if (isMouseOverButton(mouseX, mouseY, &quitButton)) {
                            menuRunning = 0;
                        }
                    }
                    break;
            }
        }
    }
}


int main(int argc, char **argv) {
    App *app = initSDL();

    if (app == NULL) {
        SDL_ExitWithError("Initialisation SDL a échoué", NULL, NULL, NULL, NULL);
    }

    srand(time(NULL)); 
    
    showMainMenu(app);

    SDL_Exit(app, NULL, NULL, NULL);

    return EXIT_SUCCESS;
}
