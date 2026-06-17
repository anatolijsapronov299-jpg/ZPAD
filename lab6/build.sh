#!/bin/bash

set -e

echo "Building Lab 6..."
mkdir -p build
cd build
cmake ..
make

echo "Build completed successfully."
