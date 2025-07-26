#include "xlsxio.h"
#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE
void getWorksheetNames(const char *file_name, const char *db_filename) {
    sqlite3 *db;
    char *db_filepath = "";
    sprintf(db_filepath, "file:/opfs/%s?vfs=opfs", db_filename);

    const int rc = sqlite3_open_v2(db_filepath, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK) {
        throwError("Error opening DB file");
        return;
    }

    xlsxioreader workbook = xlsxioread_open(file_name);
    if (workbook == NULL) {
        sqlite3_close(db);
        throwError("Error opening file");
        return;
    }

    xlsxioreadersheetlist worksheets = xlsxioread_sheetlist_open(workbook);
    if (worksheets == NULL) {
        xlsxioread_close(workbook);
        sqlite3_close(db);
        throwError("Error fetching worksheets");
        return;
    }

    const XLSXIOCHAR *sheet_name;
    while ((sheet_name = xlsxioread_sheetlist_next(worksheets)) != NULL) {
        printf("%s\n", sheet_name);
    }

    xlsxioread_sheetlist_close(worksheets);
    xlsxioread_close(workbook);
    sqlite3_close(db);
}
