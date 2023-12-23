#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
/*
Compilation du programme :
gcc src/main.c -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2
bin\prog.exe
*/

void SDL_ExitWithError(const char *message);

int main(int argc, char **argv) {

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    //Lancement SDL
    if(SDL_Init(SDL_INIT_VIDEO) !=0)
        SDL_ExitWithError("Initialisation de SDL");

    //Création fenêtre + rendu
    if(SDL_CreateWindowAndRenderer(800,600, 0, &window, &renderer) != 0)
        SDL_ExitWithError("Impossible de creer la fenetre et le rendu");

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
                            continue;;
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
    
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(EXIT_SUCCESS);
}

void SDL_ExitWithError(const char *message) {
    SDL_Log("Eror : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

