#ifndef _DATABASE_H_
#define _DATABASE_H_

#include "config.h"
#include "record.h"

/* Opens db. Return sqlite3 error code. SQLITE_OK (0) on succces */
int db_open(void);

/* Closes db. Return sqlite3 error code. SQLITE_OK (0) on succces */
int db_close(void);

/* Saves game results in the db. Return sqlite3 error code. SQLITE_OK (0) on
 * succces */
int db_save_record(double time, Difficulty difficulty);

/* Fetches all the data for the requested difficulty into dest */
int db_fetch_all_by_difficulty(Difficulty diff, r_array *dest);

#endif
