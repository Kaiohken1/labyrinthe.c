#include <stdio.h>
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
    

        
    return app;
}

void SDL_Exit(App *app) {
    if (app != NULL) {
        if (app->renderer != NULL) {
            SDL_DestroyRenderer(app->renderer);
        }
        if (app->window != NULL) {
            SDL_DestroyWindow(app->window);
        }
        free(app);
    }
    SDL_Quit();
}


void SDL_ExitWithError(const char *message, App *app) {
    if (app != NULL) {
        if (app->renderer != NULL) {
            SDL_DestroyRenderer(app->renderer);
            app->renderer == NULL;
        }
        if (app->window != NULL) {
            SDL_DestroyWindow(app->window);
            app->window == NULL;
        }

        free(app);
    }
    SDL_Log("Eror : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    free(app);
    exit(EXIT_FAILURE);
}
