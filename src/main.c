#include <stdio.h>
#include "init.h"
#include "input.h"
#include "draw.h"
/*
Compilation du programme :
gcc src/main.c -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2
bin\prog.exe
*/


int main(int argc, char **argv) {
    App *app = initSDL();

    if (app == NULL) {
        return EXIT_FAILURE;
    }

    Entity *player = malloc(sizeof(Entity));

    if (player == NULL) {
        SDL_ExitWithError("Impossible de charger le joueur", app);
        return EXIT_FAILURE;
    }

    player->x = 300;
    player->y = 50;
    player->texture = loadTexture("img/full.png", app);

    while(app->programLaunched) {
        prepareScene(app);
        inputEvent(app);
        showTexture(player->texture, player->x, player->y, app);
        presentScene(app);
    }

    free(player);
    SDL_Exit(app);

    exit(EXIT_SUCCESS);
}