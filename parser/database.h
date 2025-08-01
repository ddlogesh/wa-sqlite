#ifndef DATABASE_H
#define DATABASE_H
#define WORKSHEET_LIMIT 200

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sqlite3.h>
#include <xlsxio_read.h>
#include "utils.h"

sqlite3* init_db(const char *db_filename);
bool write_worksheet(sqlite3 *db, xlsxioreadersheetlist worksheets);

#endif
