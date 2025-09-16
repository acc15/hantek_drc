#!/usr/bin/env sh

set -eaux

cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON -DBUILD_TESTING=OFF
cmake --build build
sudo cmake --install build

cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=OFF -DBUILD_TESTING=OFF
cmake --build build
sudo cmake --install build

cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DBUILD_TESTING=OFF
cmake --build build
sudo cmake --install build

cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DBUILD_TESTING=OFF
cmake --build build
sudo cmake --install build