#!/bin/bash

set -e

echo "Cleaning build directory..."
rm -rf build

echo "Configuring project..."
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

echo "Building project..."
cmake --build build

echo "Starting application..."
./build/air-alarm-service