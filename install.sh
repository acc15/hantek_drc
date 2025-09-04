#!/usr/bin/env sh

set -eaux

cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=true -DBUILD_TESTING=false
cmake --build build
sudo cmake --install build

cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=false -DBUILD_TESTING=false
cmake --build build
sudo cmake --install build

cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=true -DBUILD_TESTING=false
cmake --build build
sudo cmake --install build

cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=false -DBUILD_TESTING=false
cmake --build build
sudo cmake --install build