#!/usr/bin/env bash
set -e

SYSROOT=$(emcc -v -c - 2>&1 | sed -n 's/.*--sysroot=\([^ ]*\).*/\1/p')

rm -rf build
mkdir build && cd build

emcmake cmake .. \
  -DCMAKE_FIND_ROOT_PATH="${SYSROOT};$HOME/.camphor" \
  -DCMAKE_BUILD_TYPE=Release

emmake make
