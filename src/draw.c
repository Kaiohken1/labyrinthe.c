#include "structs.h"
#include "draw.h"
#include "init.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "maze.h"
#include <stdio.h>
#include <string.h>

void prepareScene(App *app) {
	SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
	SDL_RenderClear(app->renderer);
}

void presentScene(App *app) {
	SDL_RenderPresent(app->renderer);
}

SDL_Texture *loadTexture(char *filename, App *app) {
    char fullPath[1024];
    getBasePath(fullPath, filename, sizeof(fullPath));

    SDL_Texture *texture = IMG_LoadTexture(app->renderer, fullPath);

    if (texture == NULL) {
        SDL_ExitWithError("Chargement de l'image échoué", app, NULL, NULL, NULL);
    }
    return texture;
}





void showTexture(SDL_Texture *texture, int x, int y, int largeurTexture, int hauteurTexture, App *app) {
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    dest.w = largeurTexture; 
    dest.h = hauteurTexture; 

    if(SDL_RenderCopy(app->renderer, texture, NULL, &dest) != 0)
        SDL_ExitWithError("Impossible d'afficher la texture", app, NULL, NULL, NULL);
}

void blit(SDL_Texture *texture, int x, int y, App *app) {
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;

    // Obtenir la largeur et la hauteur de la texture pour les mettre dans dest.w et dest.h
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    // Affichage de la texture aux coordonnées spécifiées
    SDL_RenderCopy(app->renderer, texture, NULL, &dest);
}

// TTF_Font* loadFont(char *filename, int fontSize) {
//     char font[1024];
//     getBasePath(font, filename, sizeof(font));

//     return font;
// }

void drawText(App *app, const char *message, int x, int y) {
    if (message == NULL || message[0] == '\0') {
        return;
    }

    char fullPath[1024];
    getBasePath(fullPath, "font/slkscr.ttf", sizeof(fullPath));

    
    TTF_Font *font = TTF_OpenFont(fullPath, 24);
    if (font == NULL) {
        fprintf(stderr, "Erreur lors du chargement de la police: %s\n", TTF_GetError());
        return;
    }

    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, message, textColor);

    if (surface == NULL) {
        fprintf(stderr, "Erreur lors de la création de la surface: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(app->renderer, surface);
    SDL_FreeSurface(surface);

    if (texture == NULL) {
        fprintf(stderr, "Erreur lors de la création de la texture: %s\n", SDL_GetError());
        TTF_CloseFont(font);
        return;
    }

    blit(texture, x, y, app); 
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font); 
}
