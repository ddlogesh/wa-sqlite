#!/usr/bin/env bash
set -e

rm -rf build
mkdir build && cd build

emcmake cmake .. \
  -DCMAKE_BUILD_TYPE=Release

emmake make -j8
