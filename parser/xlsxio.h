#ifndef _XLSXIO_H
#define _XLSXIO_H

#include "database.h"

bool parse_worksheet(const char *file_name, sqlite3 *db);

#endif
