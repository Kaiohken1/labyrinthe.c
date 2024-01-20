#ifndef __DATABASE__
#define __DATABASE__
#include "sqlite3.h"
#include "structs.h"

void initDB();
void insertScore(const char *name, double score);
int getScoreCount(sqlite3 *db);
void deleteScore(sqlite3 *db, double newScore);
static int callback(void *NotUsed, int argc, char **argv, char **azColName);
void displayScores(App *app);
void callDB(sqlite3 *db, App *app, double time, const char *name);

#endif