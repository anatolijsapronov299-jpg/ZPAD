#!/bin/bash

set -e

echo "Building Lab 7..."
mkdir -p build
cd build
cmake ..
make

echo "Build completed successfully."
