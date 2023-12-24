#include "input.h"

 void inputEvent(App *app) {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                app->programLaunched = SDL_FALSE;
                break;

            default:
                break;
            }
        }

}    
