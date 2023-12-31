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

    if (app == NULL || maze == NULL) {
        SDL_ExitWithError("Allocation de mémoire initiale a échouée", app);
    }

    srand(time(NULL));
    generateMaze(maze);
    renderMaze(app->renderer, maze);

    while(app->programLaunched) {
        inputEvent(app);        
    }

    SDL_Exit(app, maze);

    exit(EXIT_SUCCESS);
}