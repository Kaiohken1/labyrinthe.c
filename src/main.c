#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "init.c"


/*
Compilation du programme :
gcc src/main.c -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2
bin\prog.exe
*/

int main(int argc, char **argv) {

    initSDL();

    exit(EXIT_SUCCESS);
}