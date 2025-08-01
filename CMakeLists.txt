CMAKE_MINIMUM_REQUIRED(VERSION 4.0)

PROJECT(Camphor
        VERSION 0.1.0
        DESCRIPTION "Ship your bulk import feature in minutes!"
        HOMEPAGE_URL "https://camphor.cloud"
        LANGUAGES C
)

SET(CAMPHOR_BASE "$ENV{HOME}/.camphor")
SET(SOURCES
        # sqlite
        sqlite/sqlite3.c
        sqlite/extension-functions.c

        # parser
        parser/main.c
        parser/xlsxio.c
        parser/database.c
        parser/utils.c
)

IF (EMSCRIPTEN)
    SET(CAMPHOR_HOME "${CAMPHOR_BASE}/wasm")
    LIST(APPEND SOURCES
            # wa-sqlite
            src/libauthorizer.c
            src/libfunction.c
            src/libhook.c
            src/libprogress.c
            src/libvfs.c
    )
ELSE ()
    SET(CAMPHOR_HOME "${CAMPHOR_BASE}/native")
ENDIF ()

ADD_EXECUTABLE(excel-parser ${SOURCES})
TARGET_INCLUDE_DIRECTORIES(excel-parser PRIVATE
        "${CAMPHOR_HOME}/include"
        sqlite
        src
        parser
)
TARGET_LINK_DIRECTORIES(excel-parser PRIVATE
        "${CAMPHOR_HOME}/lib"
)
TARGET_LINK_LIBRARIES(excel-parser PRIVATE
        xlsxio_read
        minizip
        expat
        z
)
TARGET_COMPILE_DEFINITIONS(excel-parser PRIVATE
        # TODO: Reduce the SQLite build size
        # Disable unused SQLite features
        SQLITE_OMIT_AUTOINIT
        SQLITE_OMIT_DECLTYPE
        SQLITE_OMIT_DEPRECATED
        SQLITE_OMIT_LOAD_EXTENSION
        SQLITE_OMIT_SHARED_CACHE

        SQLITE_DEFAULT_MEMSTATUS=0
        SQLITE_DEFAULT_WAL_SYNCHRONOUS=1
        SQLITE_DQS=0
        SQLITE_LIKE_DOESNT_MATCH_BLOBS
        SQLITE_MAX_EXPR_DEPTH=0
        SQLITE_THREADSAFE=0
        SQLITE_USE_ALLOCA
        SQLITE_ENABLE_BATCH_ATOMIC_WRITE
)

IF (EMSCRIPTEN)
    EXECUTE_PROCESS(
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/expand_json_flags.py ${CMAKE_SOURCE_DIR}/src/exported_functions.json
            OUTPUT_VARIABLE exported_functions
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    EXECUTE_PROCESS(
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/expand_json_flags.py ${CMAKE_SOURCE_DIR}/src/extra_exported_runtime_methods.json
            OUTPUT_VARIABLE exported_runtime_methods
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    # TODO: Remove unused libraries
    SET(EM_LIBRARIES
            --js-library=${EMSCRIPTEN_ROOT_PATH}/src/lib/libworkerfs.js
            --js-library=${CMAKE_SOURCE_DIR}/src/libadapters.js
            --post-js=${CMAKE_SOURCE_DIR}/src/libauthorizer.js
            --post-js=${CMAKE_SOURCE_DIR}/src/libfunction.js
            --post-js=${CMAKE_SOURCE_DIR}/src/libhook.js
            --post-js=${CMAKE_SOURCE_DIR}/src/libprogress.js
            --post-js=${CMAKE_SOURCE_DIR}/src/libvfs.js
    )
    TARGET_LINK_OPTIONS(excel-parser PRIVATE
            -sEXPORT_ES6=1
            -sEXPORT_NAME="ExcelParserModule"
            -sEXPORTED_FUNCTIONS=${exported_functions}
            -sEXPORTED_RUNTIME_METHODS=${exported_runtime_methods}
            -sENVIRONMENT=["worker"]
            -sMODULARIZE=1
            -sFORCE_FILESYSTEM=1
            -sNO_EXIT_RUNTIME=1
            -sALLOW_MEMORY_GROWTH=1
            -sWASM_BIGINT=0
            -O3
            -flto
            ${EM_LIBRARIES}
    )
ENDIF ()
