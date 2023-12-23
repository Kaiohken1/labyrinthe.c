#include <stdio.h>
#include <SDL.h>
#include "init.h"
#include "def.h"

void initSDL(void) {
    App *app = malloc(sizeof(App));

    if (app == NULL) {
    fprintf(stderr, "Erreur : Allocation de mémoire pour Maze a échoué.\n");
    exit(EXIT_FAILURE);
    }

    if(SDL_Init(SDL_INIT_VIDEO) !=0)
        SDL_ExitWithError("Initialisation de SDL", app);

    if(SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &app->window, &app->renderer) != 0)
        SDL_ExitWithError("Impossible de creer la fenetre et le rendu", app);

     SDL_bool programLaunched = SDL_TRUE;

    while (programLaunched) {
        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_b:
                            printf("Vous avez appuye sur B\n");
                            continue;

                        default:
                            continue;
                    }

                case SDL_KEYUP:
                    switch(event.key.keysym.sym) {
                        case SDLK_b:
                            printf("Vous avez relache sur B\n");
                            continue;

                        default:
                            continue;;
                    }
                case SDL_QUIT:
                    programLaunched = SDL_FALSE;
                break;

                default:
                break;
            }
        }
    }
    
    
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    SDL_Quit();
    free(app);
    exit(EXIT_SUCCESS);   
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
