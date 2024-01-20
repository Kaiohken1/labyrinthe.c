#ifndef __LEVEL__
#define __LEVEL__

#include "structs.h"

List newList();
Bool isEmptyList(List li);
List pushFrontList(List li, double x, double lastX);
List pushBackList(List li, double x);
List popFrontList(List li);
List clearList(List li);
double getTotalTime(List li);
void printList(List li);

 #endif