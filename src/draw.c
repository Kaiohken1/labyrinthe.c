#include "structs.h"

void prepareScene(App *app)
{
	SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 3);
	SDL_RenderClear(app->renderer);
}

void presentScene(App *app)
{
	SDL_RenderPresent(app->renderer);
}