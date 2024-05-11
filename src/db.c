#include <sqlite3.h>
#include <stdio.h>
#include <sys/stat.h>

#include "db.h"

static const char *DB_PATH = "./database/minesweeper.db";
static const char *CREATE_TABLE_SQL
    = "CREATE TABLE IF NOT EXISTS minesweeper(\n"
      "    id INTEGER PRIMARY KEY,\n"
      "    timestamp TEXT,\n"
      "    time REAL,\n"
      "    difficulty TEXT\n"
      ");";

/* Create ./database directory if needed */
static void create_database_dir(void);
static sqlite3 *DB;

int db_open(void)
{
    int err;
    char *error;

    create_database_dir();

    if ((err = sqlite3_open(DB_PATH, &DB))) {
        fprintf(stderr, "Can't connect to the database '%s': %s\n", DB_PATH,
                sqlite3_errmsg(DB));
        return err;
    }

    /* Creates main table if not exists */
    if ((err = sqlite3_exec(DB, CREATE_TABLE_SQL, NULL, NULL, &error))) {
        fprintf(stderr, "Error SQL: %s\n", error);
        sqlite3_free(error);
    }
    return err;
}

static void create_database_dir(void)
{
    struct stat st;
    if (stat("./database", &st) == -1) {
        if (mkdir("./database", 0755) == -1) {
            perror("Can't create directory './database'");
        }
    }
}

int db_close(void)
{
    int err = sqlite3_close(DB);
    return err;
}

int db_save_record(double time, const char *difficulty)
{
    char sql_query[2048];
    int err;
    char *error;

    sprintf(sql_query,
            "INSERT INTO minesweeper (timestamp, time, difficulty)"
            "VALUES (datetime('now', 'localtime'), %f, '%s');",
            time, difficulty);

    if ((err = sqlite3_exec(DB, sql_query, NULL, NULL, &error))) {
        fprintf(stderr, "Can't save current result\n");
        fprintf(stderr, "Error SQL: %s\n", error);
        sqlite3_free(error);
    }
    return err;
}
