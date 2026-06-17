#!/bin/bash

set -e

if [ ! -f build/lab6_app ]; then
    echo "Executable not found. Running build first..."
    ./build.sh
fi

echo "Starting Lab 6 application..."
./build/lab6_app
