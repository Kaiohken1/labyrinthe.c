#include "structs.h"
#include"draw.h"
#include "init.h"
#include <SDL_image.h>

void prepareScene(App *app) {
	SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 3);
	SDL_RenderClear(app->renderer);
}

void presentScene(App *app) {
	SDL_RenderPresent(app->renderer);
}

SDL_Texture *loadTexture(char *filename, App *app) {
	SDL_Texture *texture;

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

	texture = IMG_LoadTexture(app->renderer, filename);

	if(texture == NULL)
		SDL_ExitWithError("Chargement de l'image échoué", app);

	return texture;
}

void showTexture(SDL_Texture *texture, int x, int y, int largeurTexture, int hauteurTexture, App *app) {
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    dest.w = largeurTexture; 
    dest.h = hauteurTexture; 

    if(SDL_RenderCopy(app->renderer, texture, NULL, &dest) != 0)
        SDL_ExitWithError("Impossible d'afficher la texture", app);
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

