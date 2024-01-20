#ifndef __INPUT__
#define __INPUT__

#include "structs.h"

void inputEvent(App *app);
void doKeyDown(SDL_KeyboardEvent *event, App *app);
void doKeyUp(SDL_KeyboardEvent *event, App *app);
void getUserInput(App *app, const char *message, char *inputText, int maxLength);
#endif