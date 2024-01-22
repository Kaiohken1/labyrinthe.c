#include "input.h"
#include "def.h"
#include "draw.h"
#include <string.h>
#include <ctype.h>

void inputEvent(App *app) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                app->programLaunched = SDL_FALSE;
                break;
            
            case SDL_KEYDOWN:
                doKeyDown(&event.key, app); // Passer app en tant qu'argument
            break;

            default:
                break;
        }
    }
}






void doInput(App *app) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;

            case SDL_KEYDOWN:
                doKeyDown(&event.key, app); // Passer app en tant qu'argument
                break;

            case SDL_KEYUP:
                doKeyUp(&event.key, app); // Passer app en tant qu'argument
                break;

            default:
                break;
        }
    }
}



void doKeyDown(SDL_KeyboardEvent *event, App *app) {
    if (event->repeat == 0) {
        switch (event->keysym.scancode) {
            case SDL_SCANCODE_UP:
                app->up = 1;
                break;
            case SDL_SCANCODE_DOWN:
                app->down = 1;
                break;
            case SDL_SCANCODE_LEFT:
                app->left = 1;
                break;
            case SDL_SCANCODE_RIGHT:
                app->right = 1;
                break;
            case SDL_SCANCODE_R:
                app->restart = SDL_TRUE;
           
            default:
                break;
        }
    }
}

void doKeyUp(SDL_KeyboardEvent *event, App *app) {
    if (event->repeat == 0) {
        switch (event->keysym.scancode) {
            case SDL_SCANCODE_UP:
                app->up = 0;
                break;
            case SDL_SCANCODE_DOWN:
                app->down = 0;
                break;
            case SDL_SCANCODE_LEFT:
                app->left = 0;
                break;
            case SDL_SCANCODE_RIGHT:
                app->right = 0;
                break;
            
            default:
                break;
        }
    }
}

void getUserInput(App *app, const char *message, char *inputText, int maxLength) {
    Bool isEnteringText = TRUE;
    int textLength = 0; 
    inputText[0] = '\0';

    SDL_StartTextInput();

    while (isEnteringText && app->programLaunched) {
        SDL_Event e = {0};
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                app->programLaunched = SDL_FALSE;
                isEnteringText = FALSE;
            } else if (e.type == SDL_TEXTINPUT) {
                if(isalpha(e.text.text[0])) {
                    int newLength = textLength + strlen(e.text.text);
                    if (newLength < maxLength) {
                        strcat(inputText, e.text.text);
                        textLength = newLength;
                    }
                }
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && textLength > 0) {
                    inputText[textLength - 1] = '\0';
                    textLength--;
                } else if (e.key.keysym.sym == SDLK_RETURN) {
                    if (textLength > 0) { 
                        isEnteringText = FALSE;
                    }
                }
            }
        }

        prepareScene(app);
        drawText(app, message, 100, 50);
        drawText(app, "Entrez votre nom : ", 100, 150); 
        drawText(app, inputText, 380, 150);
        presentScene(app);
    }

    SDL_StopTextInput();
}
