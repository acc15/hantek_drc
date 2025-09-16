#!/usr/bin/env sh

set -eaux

cmake -B build/shared_debug -S . -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON -DBUILD_TESTING=OFF
cmake --build build/shared_debug
sudo cmake --install build/shared_debug

cmake -B build/static_debug -S . -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=OFF -DBUILD_TESTING=OFF
cmake --build build/static_debug
sudo cmake --install build/static_debug

cmake -B build/shared_release -S . -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DBUILD_TESTING=OFF
cmake --build build/shared_release
sudo cmake --install build/shared_release

cmake -B build/static_release -S . -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DBUILD_TESTING=OFF
cmake --build build/static_release
sudo cmake --install build/static_release
