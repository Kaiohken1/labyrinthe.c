#include "database.h"
#include <stdio.h>
#include "draw.h"
#include <string.h>

void initDB() {
    sqlite3 *db;
    char *errMsg = 0;
    int rc;

    char fullPath[1024];
    getDatabasePath(fullPath, sizeof(fullPath));

    rc = sqlite3_open(fullPath, &db);
    if (rc) {
        fprintf(stderr, "Impossible d'ouvrir la base de données: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS Scores(Id INTEGER PRIMARY KEY, Name TEXT, Score DOUBLE);";
    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur SQL: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db); 
    }

    sqlite3_close(db);
}

void insertScore(const char *name, double score) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    char fullPath[1024];
    getDatabasePath(fullPath, sizeof(fullPath));

    rc = sqlite3_open(fullPath, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Impossible d'ouvrir la base de données: %s\n", sqlite3_errmsg(db));
    }

    const char *sql = "INSERT INTO Scores (Name, Score) VALUES (?, ?);";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, score);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Erreur d'exécution: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);
}

int getScoreCount(sqlite3 *db) {
    const char *sql = "SELECT COUNT(*) FROM Scores;";
    sqlite3_stmt *stmt;
    int count = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    return count;
}

void deleteScore(sqlite3 *db, double newScore) {
    if (getScoreCount(db) >= 10) {
        double highestScore = 0;
        const char *sqlSelect = "SELECT MAX(Score) FROM Scores;";
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(db, sqlSelect, -1, &stmt, NULL) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                highestScore = sqlite3_column_double(stmt, 0);
            }
            sqlite3_finalize(stmt);
        }

        if (newScore < highestScore) {
            const char *sqlDelete = "DELETE FROM Scores WHERE Score = (SELECT MAX(Score) FROM Scores);";
            sqlite3_exec(db, sqlDelete, 0, 0, NULL);
        }
    }
}


static int callback(void *userData, int argc, char **argv, char **azColName) {
    CallbackData *data = (CallbackData *)userData;

    if (data->count >= 10) {
        return 0;
    }

    if (argc == 2) {
        sprintf(data->lines[data->count], "#%d %s ............ %s secondes", data->count + 1, argv[0], argv[1]);
        data->count++;
    }

    return 0;
}


void displayScores(App *app) {
    sqlite3 *db;
    char *errMsg = 0;
    int rc;

    char fullPath[1024];
    getDatabasePath(fullPath, sizeof(fullPath));
    
    rc = sqlite3_open(fullPath, &db);
    if (rc) {
        fprintf(stderr, "Impossible d'ouvrir la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    CallbackData *data = malloc(sizeof(CallbackData));
    // CallbackData data = { .count = 0 } 
    if (data == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        sqlite3_close(db);
        return;
    }
    data->count = 0;

    const char *sql = "SELECT Name, Score FROM Scores ORDER BY Score ASC;";
    rc = sqlite3_exec(db, sql, callback, data, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur SQL: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        prepareScene(app);
        drawText(app, "Meilleurs scores", 100, 50);
        int y = 100;
        for (int i = 0; i < data->count; i++) {
            drawText(app, data->lines[i], 100, y);
            y += 30;
        }

        drawText(app, "Appuyez sur R pour rejouer", 100, 500); 
        presentScene(app);
    }

    sqlite3_close(db);
    free(data);
}



void callDB(sqlite3 *db, App *app, double time, const char *name) {
    initDB();
    char fullPath[1024];
    getDatabasePath(fullPath, sizeof(fullPath));

    sqlite3_open(fullPath, &db);

    deleteScore(db, time);
    insertScore(name, time);
    displayScores(app);

    sqlite3_close(db);
}

void getDatabasePath(char *fullPath, size_t fullPathSize) {
    char *basePath = SDL_GetBasePath();
    if (basePath == NULL) {
        fprintf(stderr, "Erreur lors de l'obtention du chemin de base: %s\n", SDL_GetError());
        strcpy(fullPath, "mazeBDD.db");  
        return;
    }


    for (char *p = basePath; *p; p++) {
        if (*p == '\\') {
            *p = '/';
        }
    }

    char *binSegment = strstr(basePath, "/bin");
    if (binSegment != NULL) {
        *binSegment = '\0';  
    }

    snprintf(fullPath, fullPathSize, "%s/mazeBDD.db", basePath);
    SDL_free(basePath);
}

