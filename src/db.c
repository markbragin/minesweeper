#include <sqlite3.h>
#include <stdio.h>
#include <sys/stat.h>

#include "config.h"
#include "db.h"
#include "kvec.h"
#include "record.h"

static const char *DB_PATH = "./database/minesweeper.db";
static const char *CREATE_TABLE_SQL
    = "CREATE TABLE IF NOT EXISTS minesweeper(\n"
      "    id INTEGER PRIMARY KEY,\n"
      "    timestamp INTEGER,\n"
      "    time REAL,\n"
      "    difficulty INTEGER\n"
      ");"
      "PRAGMA user_version=1";

/* Create ./database directory if needed */
static void create_database_dir_(void);
static sqlite3 *db_;

int db_open(void)
{
    int err;
    char *error;

    create_database_dir_();

    if ((err = sqlite3_open(DB_PATH, &db_))) {
        fprintf(stderr, "Can't connect to the database '%s': %s\n", DB_PATH,
                sqlite3_errmsg(db_));
        return err;
    }

    /* Creates main table if not exists */
    if ((err = sqlite3_exec(db_, CREATE_TABLE_SQL, NULL, NULL, &error))) {
        fprintf(stderr, "Error SQL: %s\n", error);
        sqlite3_free(error);
    }
    return err;
}

static void create_database_dir_(void)
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
    int err = sqlite3_close(db_);
    return err;
}

int db_save_record(double time, Difficulty difficulty)
{
    char sql_query[1024];
    int err;
    char *error;

    sprintf(sql_query,
            "INSERT INTO minesweeper (timestamp, time, difficulty)"
            "VALUES (unixepoch(), %f, %d);",
            time, difficulty);

    if ((err = sqlite3_exec(db_, sql_query, NULL, NULL, &error))) {
        fprintf(stderr, "Can't save current result\n");
        fprintf(stderr, "Error SQL: %s\n", error);
        sqlite3_free(error);
    }
    return err;
}

int db_fetch_all_by_difficulty(Difficulty diff, r_array *dest)
{
    sqlite3_stmt *res;
    int rc;

    rc = sqlite3_prepare_v2(db_,
                            "SELECT timestamp, time FROM minesweeper\n"
                            "WHERE difficulty=?1 ORDER BY time",
                            -1, &res, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db_));
        return rc;
    }

    if ((rc = sqlite3_bind_int(res, 1, diff)) != SQLITE_OK) {
        fprintf(stderr, "Failed to bind: %s\n", sqlite3_errmsg(db_));
        return rc;
    }

    while ((rc = sqlite3_step(res)) == SQLITE_ROW) {
        Record r = {sqlite3_column_int(res, 0), sqlite3_column_double(res, 1)};
        kv_push(Record, *dest, r);
    }
    rc = sqlite3_finalize(res);
    return rc;
}
