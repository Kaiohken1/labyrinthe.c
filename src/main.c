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
    renderMaze(app->renderer, maze, app);
    
    player->texture = loadTexture("img/full.png", app);
    player->x = 100;
    player->y = 100;

    // Variables pour stocker la position précédente du joueur
    int prevX = player->x;
    int prevY = player->y;

    while (app->programLaunched) {
        prepareScene(app);
        inputEvent(app);
        showTexture(player->texture, player->x, player->y, 40, 40, app);
        presentScene(app);
        doInput(app);
        

        // variables prevX et prevY pour conserver la position précédente
        prevX = player->x;
        prevY = player->y;

        if (app->up && !app->down) {
            player->y -= 5; // Déplacer d'une case vers le haut
        } else if (app->down && !app->up) {
            player->y += 5; // Déplacer d'une case vers le bas
        }

        if (app->left && !app->right) {
            player->x -= 5; // Déplacer d'une case vers la gauche
        } else if (app->right && !app->left) {
            player->x += 5; // Déplacer d'une case vers la droite
        }

        // Vérification des limites de déplacement 
        //on vérifie que le joueur ne dépasse pas les limites de l'écran
        if (player->x < 0 || player->x > SCREEN_WIDTH - 100 || player->y < 0 || player->y > SCREEN_HEIGHT - 100) {
            // on restaure sa position précédente
            player->x = prevX;
            player->y = prevY;
        }

        blit(player->texture, player->x, player->y, app);

        SDL_Delay(0); // Contrôle de la vitesse de déplacement
    }

    SDL_Exit(app, maze);

    return EXIT_SUCCESS;
}

