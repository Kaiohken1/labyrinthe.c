#include <SDL.h>
#ifndef __STRUCTS__
#define __STRUCTS__

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_bool programLaunched;
    SDL_bool restart;
    int up;
	int down;
	int left;
	int right;
} App;

typedef struct {
    int x;
    int y;
    SDL_Texture *texture;
} Entity;

typedef enum {
    FALSE,
    TRUE
} Bool;

typedef struct {
    int x, y;           
    Bool isWall;   
} Cell;


typedef struct {
    int width;
    int height;
    Cell **tab;
} Maze;

typedef struct {
    Cell *cells;
    int top;
    int capacity;
} Stack;

typedef struct ListElement {
    double time;
    double totalTime;
    struct ListElement *next;
} ListElement, *List;

typedef struct {
    char lines[10][128]; 
    int count;           
} CallbackData;

#endif