#include <stdio.h>
#include <SDL_image.h>
#include "init.h"
#include "def.h"
#include "maze.h"

App *initSDL() {
    App *app = malloc(sizeof(App));

    if (app == NULL) {
        SDL_ExitWithError("Erreur : Allocation de mémoire pour l'app a echoue.\n", NULL, NULL, NULL);
    }

    if(SDL_Init(SDL_INIT_VIDEO) !=0)
        SDL_ExitWithError("Initialisation de SDL", app, NULL, NULL);

    if(SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &app->window, &app->renderer) != 0)
        SDL_ExitWithError("Impossible de creer la fenetre et le rendu", app, NULL, NULL);

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if ((IMG_Init(imgFlags) & imgFlags) != imgFlags)
        SDL_ExitWithError("Erreur d'initialisation de SDL_image", app, NULL, NULL);

    return app;
}

void appInit(App *app) {
    app->programLaunched = TRUE;
    app->up = 0;
    app->down = 0;
    app->left = 0;
    app->right = 0;
}

void SDL_Exit(App *app, Maze * maze, Entity *player) {
    if (app != NULL) {
        if (app->renderer != NULL) {
            SDL_DestroyRenderer(app->renderer);
            app->renderer = NULL;
        }
        if (app->window != NULL) {
            SDL_DestroyWindow(app->window);
            app->window = NULL;
        }
        free(app);
    }

    if (maze != NULL) {
        freeGrid(maze);
        free(maze);
    }

    if(player != NULL) {
        free(player);
    }

    IMG_Quit();
    SDL_Quit();
}


void SDL_ExitWithError(const char *message, App *app, Maze *maze, Entity *player) {
    if (app != NULL) {
        if (app->renderer != NULL) {
            SDL_DestroyRenderer(app->renderer);
            app->renderer = NULL;
        }
        if (app->window != NULL) {
            SDL_DestroyWindow(app->window);
            app->window = NULL;
        }
        free(app);
    }

    if (maze != NULL) {
        freeGrid(maze);
        free(maze);
    }

    if(player != NULL) {
        free(player);
    }

    IMG_Quit();
    SDL_Log("Error : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}
