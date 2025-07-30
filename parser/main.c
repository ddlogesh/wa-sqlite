#include "xlsxio.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

EMSCRIPTEN_KEEPALIVE
bool load_worksheet(const char *file_name, const char *db_filename) {
    sqlite3 *db = init_db(db_filename);
    if (!db) return false;

    bool result = parse_worksheet(file_name, db);
    sqlite3_close(db);
    return result;
}

EMSCRIPTEN_KEEPALIVE
void *getSqliteFree() {
    return sqlite3_free;
}

int main() {
    sqlite3_initialize();
    return 0;
}
