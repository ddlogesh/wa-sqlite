#include "xlsxio.h"

bool parse_worksheet(const char *file_name, sqlite3 *db) {
    xlsxioreader workbook = xlsxioread_open(file_name);
    if (workbook == NULL) {
        throw_error("Error opening file");
        return false;
    }

    xlsxioreadersheetlist worksheets = xlsxioread_sheetlist_open(workbook);
    if (worksheets == NULL) {
        xlsxioread_close(workbook);
        throw_error("Error fetching worksheets");
        return false;
    }

    bool result = write_worksheet(db, worksheets);
    xlsxioread_sheetlist_close(worksheets);
    xlsxioread_close(workbook);
    return result;
}
