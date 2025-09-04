#!/usr/bin/env sh

cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=true
cmake --build build
sudo cmake --install build

cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=false
cmake --build build
sudo cmake --install build

cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=true
cmake --build build
sudo cmake --install build

cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=false
cmake --build build
sudo cmake --install build