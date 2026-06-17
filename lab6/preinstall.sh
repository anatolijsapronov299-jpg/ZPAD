#!/bin/bash

set -e

echo "Installing dependencies for Lab 6..."
sudo apt update
sudo apt install -y libopencv-dev cmake g++ build-essential

echo "Dependencies installed successfully."
