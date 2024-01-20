#include "structs.h"
#include "draw.h"
#include "init.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "maze.h"

void prepareScene(App *app) {
	SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
	SDL_RenderClear(app->renderer);
}

void presentScene(App *app) {
	SDL_RenderPresent(app->renderer);
}

SDL_Texture *loadTexture(char *filename, App *app) {
	SDL_Texture *texture;

	texture = IMG_LoadTexture(app->renderer, filename);

	if(texture == NULL)
		SDL_ExitWithError("Chargement de l'image échoué", app, NULL, NULL);

	return texture;
}

void showTexture(SDL_Texture *texture, int x, int y, int largeurTexture, int hauteurTexture, App *app) {
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    dest.w = largeurTexture; 
    dest.h = hauteurTexture; 

    if(SDL_RenderCopy(app->renderer, texture, NULL, &dest) != 0)
        SDL_ExitWithError("Impossible d'afficher la texture", app, NULL, NULL);
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

void drawText(App *app, const char *message) {
    TTF_Font *font = TTF_OpenFont("font/slkscr.ttf", 24);
    if (!font) {
        SDL_ExitWithError("Erreur lors du chargement de la police", app, NULL, NULL);
    }

    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, message, textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app->renderer, surface);
    SDL_FreeSurface(surface);

    prepareScene(app);
    blit(texture, 100, 100, app); 
    presentScene(app);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font); 
}


