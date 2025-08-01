# Excel Parser
This is a WebAssembly build of SQLite with support for writing SQLite virtual filesystems completely in Javascript. This allows alternative browser storage options such as IndexedDB and Origin Private File System.

## Prerequisites
Emscripten should be installed via emsdk or Homebrew.
```shell
> emcc -v
emcc (Emscripten gcc/clang-like replacement + linker emulating GNU ld) 4.0.11-git
```

## Build

```shell
git clone https://github.com/ddlogesh/excel-parser
cd excel-parser
./run.sh
```

Note: The default build via Makefile produces ES6 modules + WASM, synchronous and asynchronous (using Asyncify and JSPI) in `dist/`.
