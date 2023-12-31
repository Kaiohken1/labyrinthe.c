#include "structs.h"
#include "draw.h"
#include "init.h"
#include <SDL_image.h>
#include "maze.h"

void prepareScene(App *app) {
	SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 0);
	SDL_RenderClear(app->renderer);
}

void presentScene(App *app) {
	SDL_RenderPresent(app->renderer);
}

SDL_Texture *loadTexture(char *filename, App *app) {
	SDL_Texture *texture;

	texture = IMG_LoadTexture(app->renderer, filename);

	if(texture == NULL)
		SDL_ExitWithError("Chargement de l'image échoué", app);

	return texture;
}

void showTexture(SDL_Texture *texture, int x, int y, App *app) {
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	if(SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h) !=0)
		SDL_ExitWithError("Impossible de charger la texture", app);

	if(SDL_RenderCopy(app->renderer, texture, NULL, &dest) !=0)
		SDL_ExitWithError("Impossible d'afficher la texture", app);
}