#include <SDL.h>
#ifndef __STRUCTS__
#define __STRUCTS__

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_bool programLaunched;
} App;

typedef struct {
    int x;
    int y;
    SDL_Texture *texture;
} Entity;

#endif