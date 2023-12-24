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

    while(app->programLaunched) {
        prepareScene(app);
        inputEvent(app);
        presentScene(app);
    }

    SDL_Exit(app);

    exit(EXIT_SUCCESS);
}