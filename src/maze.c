#include "maze.h"
#include "def.h"
#include "init.h"
#include "draw.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void createGrid(Maze *maze) {
    maze->width = MAZE_WIDTH;
    maze->height = MAZE_HEIGHT; 
    maze->tab = malloc(maze->width * maze->height * sizeof(Cell));

    if (maze->tab == NULL) {
        fprintf(stderr, "Erreur d'allocation du tableau");
        exit(EXIT_FAILURE);
    }

    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            int index = y * maze->width + x;
            maze->tab[index].x = x;
            maze->tab[index].y = y;
            maze->tab[index].isWall = TRUE;
        }
    }
}



Bool canDelete(Maze *maze, Cell *cell, int dx, int dy) {
    int targetX = cell->x + dx;
    int targetY = cell->y + dy;

    Cell targetCell;
    targetCell.x = targetX;
    targetCell.y = targetY;

    if (targetX < 1 || targetX >= maze->width - 1|| targetY < 1 || targetY >= maze->height - 1) {
    return FALSE;
    }

    int checkX[6], checkY[6];

    if (dx == 0 && dy == -1) { // Bas
        checkX[0] = targetCell.x - 1;    checkY[0] = targetCell.y;
        checkX[1] = targetCell.x - 1;    checkY[1] = targetCell.y - 1;
        checkX[2] = targetCell.x;        checkY[2] = targetCell.y - 1;
        checkX[3] = targetCell.x + 1;    checkY[3] = targetCell.y - 1;
        checkX[4] = targetCell.x + 1;    checkY[4] = targetCell.y;
    } else if (dx == 0 && dy == 1) { // Haut
        checkX[0] = targetCell.x - 1;    checkY[0] = targetCell.y;
        checkX[1] = targetCell.x - 1;    checkY[1] = targetCell.y + 1;
        checkX[2] = targetCell.x;        checkY[2] = targetCell.y + 1;
        checkX[3] = targetCell.x + 1;    checkY[3] = targetCell.y + 1;
        checkX[4] = targetCell.x + 1;    checkY[4] = targetCell.y;
    } else if (dx == -1 && dy == 0) { // Gauche
        checkX[0] = targetCell.x;        checkY[0] = targetCell.y + 1;
        checkX[1] = targetCell.x - 1;    checkY[1] = targetCell.y + 1;
        checkX[2] = targetCell.x - 1;    checkY[2] = targetCell.y;
        checkX[3] = targetCell.x - 1;    checkY[3] = targetCell.y - 1;
        checkX[4] = targetCell.x;        checkY[4] = targetCell.y - 1;
    } else if (dx == 1 && dy == 0) { // Droite
        checkX[0] = targetCell.x;        checkY[0] = targetCell.y + 1;
        checkX[1] = targetCell.x + 1;    checkY[1] = targetCell.y + 1;
        checkX[2] = targetCell.x + 1;    checkY[2] = targetCell.y;
        checkX[3] = targetCell.x + 1;    checkY[3] = targetCell.y - 1;
        checkX[4] = targetCell.x;        checkY[4] = targetCell.y - 1;
    }


    checkX[5] = targetX;
    checkY[5] = targetY;

    for (int i = 0; i < 6; i++) {
        if (checkX[i] < 1 || checkX[i] >= maze->width - 1 || checkY[i] < 1 || checkY[i] >= maze->height - 1) {
            continue;
        }

        int index = checkY[i] * maze->width + checkX[i];
        if (!maze->tab[index].isWall) {
            return FALSE;
        }
    }

    return TRUE; 
}



void deleteWall(Maze *maze, Cell *cell, int dx, int dy) {

    int newX = cell->x + dx;
    int newY = cell->y + dy;
    if (newX >= 0 && newY >= 0 && newX < maze->width && newY < maze->height) {
        int index = newY * maze->width + newX;
        maze->tab[index].isWall = FALSE;
    }
}

void moveCursor(Cell *cursor, int dx, int dy) {
    cursor->x += dx;
    cursor->y += dy;
}

// void createEntry(Maze *maze) {
//     int topY = 0;

//     for (int topX = 0; topX < 2; topX++) {
//         if (topX >= 0 && topY >= 0 && topX < maze->width && topY < maze->height) {
//             int index = topY * maze->width + topX;
//             maze->tab[index].isWall = FALSE;
//         }
//     }
// }


void createExit(Maze *maze) {
    int botY = maze->height - 2;

    for (int botX = maze->width - 1; botX >= maze->width - 2; botX--) {
        if (botX >= 0 && botY >= 0 && botX < maze->width && botY < maze->height) {
            int index = botY * maze->width + botX;
            maze->tab[index].isWall = FALSE;
        }
    }
}

void generateMaze(Maze *maze) {
    createGrid(maze);
    Cell cursor = {1, 1};
    deleteWall(maze, &cursor, 0, 0);

    Stack stack;
    initStack(&stack, maze->width * maze->height); 

    while (TRUE) {
        int paths[4][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}};
        int pathNb = 0;

        if (canDelete(maze, &cursor, 1, 0)) { // Droite
            paths[pathNb][0] = 1;  // dx
            paths[pathNb][1] = 0;  // dy
            pathNb++;
        }
        if (canDelete(maze, &cursor, -1, 0)) { // Gauche
            paths[pathNb][0] = -1; // dx
            paths[pathNb][1] = 0;  // dy
            pathNb++;
        }
        if (canDelete(maze, &cursor, 0, 1)) { // Bas
            paths[pathNb][0] = 0;  // dx
            paths[pathNb][1] = 1;  // dy
            pathNb++;
        }
        if (canDelete(maze, &cursor, 0, -1)) { // Haut
            paths[pathNb][0] = 0;  // dx
            paths[pathNb][1] = -1; // dy
            pathNb++;
        }

        if (pathNb > 0) {
            int choice = rand() % pathNb; 
            int dx = paths[choice][0];
            int dy = paths[choice][1];

            deleteWall(maze, &cursor, dx, dy);
            push(&stack, cursor);
            moveCursor(&cursor, dx, dy);
        } else {
            if (isStackEmpty(&stack)) {
                createExit(maze);
                break; 
            } else {
                Cell lastPosition;
                if (pop(&stack, &lastPosition)) {
                    cursor = lastPosition; 
                }
            }
        }
    }

    freeStack(&stack);
    free(maze->tab); 
}



void initStack(Stack *stack, int capacity) {
    stack->cells = malloc(capacity * sizeof(Cell));
    stack->top = -1;
    stack->capacity = capacity;
}

Bool isStackEmpty(Stack *stack) {
    return stack->top == -1;
}

Bool isStackFull(Stack *stack) {
    return stack->top == stack->capacity - 1;
}

void push(Stack *stack, Cell cell) {
    if (isStackFull(stack)) {
        return;
    }
    stack->cells[++stack->top] = cell;
}

Bool pop(Stack *stack, Cell *cell) {
    if (isStackEmpty(stack)) {
        return FALSE;
    }
    *cell = stack->cells[stack->top--];
    return TRUE;
}

void freeStack(Stack *stack) {
    free(stack->cells);
    stack->cells = NULL;
}


void renderMaze(SDL_Renderer *renderer, Maze *maze, App *app) {
    if (renderer == NULL || maze == NULL || maze->tab == NULL) {
        fprintf(stderr, "Erreur : pointeur non initialisé.\n");
        return;
    }

    int centerX = (SCREEN_WIDTH / 2) - (maze->width * CELL_SIZE) / 2;
    int centerY = (SCREEN_HEIGHT / 2) - (maze->height * CELL_SIZE) / 2;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_Texture *texture = loadTexture("img/stone.jpg", app);

    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            int index = y * maze->width + x;
            if (maze->tab[index].isWall) {
                SDL_Rect wallRect;
                wallRect.x = centerX + maze->tab[index].x * CELL_SIZE;
                wallRect.y = centerY + maze->tab[index].y * CELL_SIZE;
                wallRect.w = CELL_SIZE;
                wallRect.h = CELL_SIZE;
                if (SDL_RenderCopy(renderer, texture, NULL, &wallRect) != 0)
                    SDL_ExitWithError("Impossible d'afficher la texture", app);
            } else {
                SDL_Rect passageRect;
                passageRect.x = centerX + maze->tab[index].x * CELL_SIZE;
                passageRect.y = centerY + maze->tab[index].y * CELL_SIZE;
                passageRect.w = CELL_SIZE;
                passageRect.h = CELL_SIZE;

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &passageRect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}
