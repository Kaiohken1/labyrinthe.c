#include <stdio.h>
#include <SDL_image.h>
#include "init.h"
#include "def.h"

App *initSDL() {
    App *app = malloc(sizeof(App));

    if (app == NULL) {
    fprintf(stderr, "Erreur : Allocation de mémoire pour l'app a echoue.\n");
    exit(EXIT_FAILURE);
    }

    if(SDL_Init(SDL_INIT_VIDEO) !=0)
        SDL_ExitWithError("Initialisation de SDL", app);

    if(SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &app->window, &app->renderer) != 0)
        SDL_ExitWithError("Impossible de creer la fenetre et le rendu", app);

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    return app;
}

void SDL_Exit(App *app) {
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
    IMG_Quit();
    SDL_Quit();
}


void SDL_ExitWithError(const char *message, App *app) {
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
    IMG_Quit();
    SDL_Log("Error : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}
