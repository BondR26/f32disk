#!/bin/bash

mkdir build
mkdir bin
cd build
# Run CMake to configure the project
if cmake -DCMAKE_BUILD_TYPE=Release ..; then
    echo "CMake configuration successful"
    
    # Build the project
    if cmake --build .; then
        echo "Build successful"
    else
        echo "Error: Failed to build the project"
        exit 1
    fi
else
    echo "Error: CMake configuration failed"
    exit 1
fi
