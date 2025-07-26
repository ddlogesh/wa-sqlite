CMAKE_MINIMUM_REQUIRED(VERSION 4.0)
project(wa-sqlite C)

# Source files
set(C_SOURCES
        sqlite/build/sqlite3.c
        sqlite/extension-functions.c
        src/main.c
        src/libauthorizer.c
        src/libfunction.c
        src/libhook.c
        src/libprogress.c
        src/libvfs.c
)

# Include directories
include_directories(
        src
        sqlite/build
)

# Common defines
add_definitions(
        -DSQLITE_DEFAULT_MEMSTATUS=0
        -DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1
        -DSQLITE_DQS=0
        -DSQLITE_LIKE_DOESNT_MATCH_BLOBS
        -DSQLITE_MAX_EXPR_DEPTH=0
        -DSQLITE_OMIT_AUTOINIT
        -DSQLITE_OMIT_DECLTYPE
        -DSQLITE_OMIT_DEPRECATED
        -DSQLITE_OMIT_LOAD_EXTENSION
        -DSQLITE_OMIT_SHARED_CACHE
        -DSQLITE_THREADSAFE=0
        -DSQLITE_USE_ALLOCA
        -DSQLITE_ENABLE_BATCH_ATOMIC_WRITE
)

# Emscripten flags
set(EMFLAGS_COMMON
        "-sALLOW_MEMORY_GROWTH=1"
        "-sWASM=1"
        "-sINVOKE_RUN"
        "-sENVIRONMENT=web,worker"
        "-sSTACK_SIZE=512KB"
        "-sWASM_BIGINT=0"
)

set(EMFLAGS_INTERFACES
        "-sEXPORTED_FUNCTIONS=src/exported_functions.json"
        "-sEXPORTED_RUNTIME_METHODS=src/extra_exported_runtime_methods.json"
)

set(EMFLAGS_LIBRARIES
        "--js-library src/libadapters.js"
        "--post-js src/libauthorizer.js"
        "--post-js src/libfunction.js"
        "--post-js src/libhook.js"
        "--post-js src/libprogress.js"
        "--post-js src/libvfs.js"
)

set(EMFLAGS_ASYNCIFY_COMMON
        "-sASYNCIFY"
        "-sASYNCIFY_IMPORTS=src/asyncify_imports.json"
)

set(EMFLAGS_BUILD "-Oz" "-flto")
set(EMFLAGS_ASYNCIFY "-sASYNCIFY_STACK_SIZE=16384")

add_library(wa-sqlite STATIC ${C_SOURCES})
set_target_properties(wa-sqlite-static PROPERTIES
        OUTPUT_NAME "wa-sqlite"
)

add_library(wa-sqlite-js ${C_SOURCES})
set_target_properties(wa-sqlite-js PROPERTIES
        OUTPUT_NAME "wa-sqlite"
        SUFFIX ".js"
)
target_link_options(wa-sqlite-js PRIVATE 
        ${EMFLAGS_BUILD}
        ${EMFLAGS_COMMON}
        ${EMFLAGS_INTERFACES}
        ${EMFLAGS_LIBRARIES}
        ${EMFLAGS_ASYNCIFY_COMMON}
        ${EMFLAGS_ASYNCIFY}
)
