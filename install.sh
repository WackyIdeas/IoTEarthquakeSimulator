#!/bin/bash

USE_NINJA=

if [[ "$*" == *"--ninja"* ]]
then
    if [[ -z "$(command -v ninja)" ]]; then
        echo "Attempted to build using Ninja, but Ninja was not found on the system. Falling back to GNU Make."
    else
        echo "Compiling using Ninja"
        USE_NINJA="-G Ninja"
    fi
fi

mkdir -p build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release .. $USE_NINJA
cmake --build .
cd ..
cp -r src/data build/src/
