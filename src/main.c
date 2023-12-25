#include <stdio.h>
#include "init.h"
#include "input.h"
#include "draw.h"
/*
Compilation du programme :
gcc src/*.c -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
bin\prog.exe
*/


int main(int argc, char **argv) {
    App *app = initSDL();

    if (app == NULL) {
        return EXIT_FAILURE;
    }

    while(app->programLaunched) {
        prepareScene(app);
        inputEvent(app);
        presentScene(app);
    }

    SDL_Exit(app);

    exit(EXIT_SUCCESS);
}