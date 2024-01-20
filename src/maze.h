#ifndef __MAZE__
#define __MAZE__
#include "structs.h"

void createGrid(Maze *maze);
Bool canDelete(Maze *maze, Cell *cell, int dx, int dy);
void deleteWall(Maze *maze, Cell *cell, int dx, int dy);
void moveCursor(Cell *taupe, int dx, int dy);
void generateMaze(Maze *maze);
void initStack(Stack *stack, int capacity);
Bool isStackEmpty(Stack *stack);
Bool isStackFull(Stack *stack);
void push(Stack *stack, Cell cell);
Bool pop(Stack *stack, Cell *cell);
void freeStack(Stack *stack);
void renderMaze(SDL_Renderer *renderer, Maze *maze, App * app);
void createEntry(Maze *maze);
void createExit(Maze *maze);
void freeGrid(Maze *maze);
Bool checkCollision(Maze *maze, Entity *entity, int newX, int newY);
#endif