#include <SDL.h>
#ifndef __STRUCTS__
#define __STRUCTS__

typedef struct {
    SDL_Renderer  *renderer;
    SDL_Window  *window;
} App;

#endif