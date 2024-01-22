#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

#define MAX_LINE_LENGTH 256

int parseIniFileint(char *filename, char *key) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Impossible d'ouvrir le fichier de configuration");
        return -1;
    }

    int value = -1; // Default value
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file)) {
        char keyFromFile[MAX_LINE_LENGTH];
        int tempValue;

        // Utilisation de sscanf pour extraire la clé et la valeur
        if (sscanf(line, "%s = %d", keyFromFile, &tempValue) == 2) {
            if (strcmp(keyFromFile, key) == 0) {
                value = tempValue;
                break;
            }
        }
    }

    fclose(file);

    if (value != -1) {
        printf("%s : %d\n", key, value);
    } else {
        printf("%s non trouvé dans le fichier de configuration.\n", key);
    }

    return value;
}

