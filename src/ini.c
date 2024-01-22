#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

#define MAX_LINE_LENGTH 256

int parseIniFile(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Impossible d'ouvrir le fichier de configuration");
        return -1; // Return a value to indicate an error
    }

    int NumberOfLevels = -1; // Default value

    char line[MAX_LINE_LENGTH]; // Use MAX_LINE_LENGTH here
    while (fgets(line, sizeof(line), file)) {
        // Search for the line containing NumberOfLevels
        if (strstr(line, "NumberOfLevels") != NULL) {
            // Use sscanf to extract the value
            if (sscanf(line, "NumberOfLevels = %d", &NumberOfLevels) == 1) {
                break; // Exit the loop once the value is found
            }
        }
    }

    fclose(file);

    if (NumberOfLevels != -1) {
        printf("Nombre de niveaux : %d\n", NumberOfLevels);
    } else {
        printf("NumberOfLevels non trouvé dans le fichier de configuration.\n");
    }

    return NumberOfLevels; 
}

