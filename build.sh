#!/bin/sh
set -e
clang -O2 build.c -o build
./build "$@"
