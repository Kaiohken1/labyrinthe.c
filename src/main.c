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


int main(int argc, char **argv) {
    App *app = initSDL();
    Maze *maze = malloc(sizeof(Maze));
    Entity *player = malloc(sizeof(Entity));

    if (app == NULL || maze == NULL || player == NULL) {
        SDL_ExitWithError("Allocation de mémoire initiale a échouée", app);
    }

    srand(time(NULL)); 
    generateMaze(maze);
    player->texture = loadTexture("img/full.png", app);
    player->x = 100;
    player->y = 100;

    // Variables pour stocker la position précédente du joueur
    int prevX = player->x;
    int prevY = player->y;

    SDL_Texture *buffer = SDL_CreateTexture(app->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_SetRenderTarget(app->renderer, buffer);
    renderMaze(app->renderer, maze, app);
    SDL_SetRenderTarget(app->renderer, NULL);

    while (app->programLaunched) {     
        inputEvent(app);

        // variables prevX et prevY pour conserver la position précédente
        prevX = player->x;
        prevY = player->y;

        if (app->up && !app->down) {
            player->y -= 1; // Déplacer d'une case vers le haut
        } else if (app->down && !app->up) {
            player->y += 1; // Déplacer d'une case vers le bas
        }

        if (app->left && !app->right) {
            player->x -= 1; // Déplacer d'une case vers la gauche
        } else if (app->right && !app->left) {
            player->x += 1; // Déplacer d'une case vers la droite
        }

        //on vérifie que le joueur ne dépasse pas les limites de l'écran
        if (player->x < 0 || player->x > SCREEN_WIDTH - 100 || player->y < 0 || player->y > SCREEN_HEIGHT - 100) {
            // on restaure sa position précédente
            player->x = prevX;
            player->y = prevY;
        }

        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
        SDL_RenderClear(app->renderer);

        SDL_RenderCopy(app->renderer, buffer, NULL, NULL);

        blit(player->texture, player->x, player->y, app);

        presentScene(app);
        doInput(app);
    }
    freeGrid(maze);
    SDL_Exit(app, maze);
    return EXIT_SUCCESS;
}


