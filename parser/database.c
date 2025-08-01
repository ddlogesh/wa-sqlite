#include "database.h"

sqlite3* init_db(const char *db_filename) {
    sqlite3 *db;
    const int rc = sqlite3_open_v2(db_filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK) {
        throw_error("Error opening DB file");
        return NULL;
    }
    return db;
}

bool write_worksheet(sqlite3 *db, xlsxioreadersheetlist worksheets) {
    const char *query = "BEGIN; DELETE FROM worksheets; INSERT INTO worksheets (name) VALUES ";
    size_t buf_size = 1024;
    char *sql = malloc(buf_size);
    size_t len = strlen(query);
    memcpy(sql, query, len);

    const char *commit = "COMMIT;";
    size_t commit_len = strlen(commit);

    int i = 0;
    const XLSXIOCHAR *sheet_name;
    while ((sheet_name = xlsxioread_sheetlist_next(worksheets)) != NULL && i < WORKSHEET_LIMIT) {
        const size_t sheet_len = strlen(sheet_name);
        const size_t extra_len = 4 + sheet_len + 2 + commit_len; // `('SHEET'), `
        if (len + extra_len + 1 >= buf_size) {
            buf_size = (len + extra_len + 1) * 2;
            char *new_sql = realloc(sql, buf_size);
            sql = new_sql;
        }

        sql[len++] = '(';
        sql[len++] = '\'';
        memcpy(sql + len, sheet_name, sheet_len);
        len += sheet_len;
        sql[len++] = '\'';
        sql[len++] = ')';
        sql[len++] = ',';
        sql[len++] = ' ';

        i++;
    }
    sql[len - 2] = ';';
    memcpy(sql + len, commit, commit_len);
    sql[len + commit_len] = '\0';

    char *err_msg;
    const int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    bool result = rc == SQLITE_OK;
    if (!result) throw_error(err_msg);

    free(err_msg);
    free(sql);

    return result;
}
