#!/usr/bin/env bash

# Linux build script
# -------------------------------------------------------
# Options:
#   build_linux.sh "path/to/build" Debug/Release
# Example:
#   build_linux.sh build Debug
#
# The VCPKG toolchain CMake file is typically under "vcpkg\scripts\buildsystems"

set -e

if [ "$2" != "Debug" ] && [ "$2" != "Release" ]; then
    echo "$2 must be either Debug or Release"
    exit 1
fi

if [ ! -d "$1" ]; then
    echo "Creating build path at $1"
    mkdir -p "$1" || { echo "Failed to create directory at $1"; exit 1; }
fi

cd "$1" || exit 1

if [ "$2" = "Debug" ]; then
    echo "Building with Debug configuration"

    cmake .. -DCMAKE_BUILD_TYPE=Debug
    cmake --build . -- -j$(nproc)
elif [ "$2" = "Release" ]; then
    echo "Building with Release configuration"

    cmake .. -DCMAKE_BUILD_TYPE=Release
    cmake --build . -- -j$(nproc)
fi

cd ..