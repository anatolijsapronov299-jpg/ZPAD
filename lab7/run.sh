#!/bin/bash

set -e

if [ ! -f models/deploy.prototxt ] || [ ! -f models/res10_300x300_ssd_iter_140000.caffemodel ]; then
    echo "Model files are missing. Running preinstall.sh first..."
    ./preinstall.sh
fi

if [ ! -f build/lab7_app ]; then
    echo "Executable not found. Running build.sh first..."
    ./build.sh
fi

echo "Starting Lab 7 application..."
./build/lab7_app
