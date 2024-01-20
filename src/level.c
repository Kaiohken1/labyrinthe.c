#include "level.h"
#include <stdio.h>
#include <stdlib.h>

List newList() { 
    return NULL;
}

Bool isEmptyList(List li) {
	if(li == NULL)
		return TRUE;
	return FALSE;
}

List pushFrontList(List li, double x, double lastX) {
	ListElement *element;

	element = malloc(sizeof(*element));

	if(element == NULL) {
		fprintf(stderr, "Erreur : problème allocation dynamique.\n");
		exit(EXIT_FAILURE);
	}

	element->time = x;
    element->totalTime = lastX;

	if(isEmptyList(li)) {
		element->next = NULL;
	} else {
		element->next = li;
	}
	return element;
}


List pushBackList(List li, double x) {
    ListElement *element;

    element = malloc(sizeof(*element));
    if(element == NULL) {
        fprintf(stderr, "Erreur : probleme allocation dynamique.\n");
        exit(EXIT_FAILURE);
    }

    element->time = x;
    element->next = NULL;

    if (isEmptyList(li)) {
        element->totalTime = x;
        return element;
    }

    ListElement *temp = li;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    element->totalTime = temp->totalTime + x;

    temp->next = element;
    return li;
}


List popFrontList(List li) {
	ListElement *element;

	if(isEmptyList(li)) {
		fprintf(stderr, "Erreur : la liste est vide, impossible de retirer un élément.\n");
		exit(EXIT_FAILURE);
	}

	element = li->next;
	free(li);

	return element;
}

List clearList(List li) {
	while(li != NULL) {
		li = popFrontList(li);
	}

	return li;
}

double getTotalTime(List li) {
    if (isEmptyList(li)) {
        fprintf(stderr, "Erreur : la liste est vide.\n");
        exit(EXIT_FAILURE);
    }

    ListElement *current = li;
    while (current->next != NULL) {
        current = current->next;
    }
    return current->totalTime;
}
