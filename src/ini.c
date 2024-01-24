#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "init.h"

#define MAX_LINE_LENGTH 256

int parseIniFileint(char *key, App *app) {
    char fullPath[1024];
    getBasePath(fullPath, "bin/config.ini", sizeof(fullPath));

    FILE *file = fopen(fullPath, "r");
    if (file == NULL) {
        SDL_ExitWithError("Impossible d'ouvrir le fichier de configuration", app, NULL, NULL, NULL);
    }

    int value = -1;
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file)) {
        char keyFromFile[MAX_LINE_LENGTH];
        int tempValue;

        if (sscanf(line, "%s = %d", keyFromFile, &tempValue) == 2) {
            if (strcmp(keyFromFile, key) == 0) {
                value = tempValue;
                break;
            }
        }
    }

    fclose(file);

    if (value == -1) {
        SDL_ExitWithError("Valeur de configuration non reconnue", app, NULL, NULL, NULL);
    }

    return value;
}


char* parseIniFileString(char *key, App *app) {
    char fullPath[1024];
    getBasePath(fullPath, "bin/config.ini", sizeof(fullPath));

    FILE *file = fopen(fullPath, "r");
    if (file == NULL) {
        SDL_ExitWithError("Impossible d'ouvrir le fichier de configuration", app, NULL, NULL, NULL);
    }

    char *value = NULL;
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file)) {
        char keyFromFile[MAX_LINE_LENGTH];
        char tempValue[MAX_LINE_LENGTH];

        if (sscanf(line, "%s = %s", keyFromFile, tempValue) == 2) {
            if (strcmp(keyFromFile, key) == 0) {
                value = strdup(tempValue);
                break;
            }
        }
    }

    fclose(file);

    if (value == NULL) {
        SDL_ExitWithError("Impossible d'ouvrir le fichier de configuration", app, NULL, NULL, NULL);
    }

    return value;
}