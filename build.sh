#!/bin/sh

set -ex

CFLAGS="-Wall -Wextra `pkg-config --cflags raylib`"
LIBS="`pkg-config --libs raylib` -lm -ldl -lpthread"

clang $CFLAGS -o musicalizer main.c $LIBS
