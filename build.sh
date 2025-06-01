#!/bin/bash

# Exit immediately if a command fails
set -e

# Path to Clang (consider to change this path to your own LLVM path if its wrong)
CLANG_PATH="/usr/bin"

# Default build type (Debug if not specified)
BUILD_TYPE="${1:-Debug}"

# Normalize input (uppercase first letter)
BUILD_TYPE="$(tr '[:lower:]' '[:upper:]' <<< ${BUILD_TYPE:0:1})${BUILD_TYPE:1}"

# Set build folder name based on type
if [[ "$BUILD_TYPE" == "Debug" ]]; then
  BUILD_DIR="build/debug"
elif [[ "$BUILD_TYPE" == "Release" ]]; then
  BUILD_DIR="build/release"
else
  echo "Unknown build type: $BUILD_TYPE"
  echo "Usage: $0 [Debug|Release]"
  exit 1
fi

# Create build directory if it doesn't exist
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Run CMake with Ninja and Clang
cmake -G "Ninja" \
  -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
  -DCMAKE_C_COMPILER="$CLANG_PATH/clang" \
  -DCMAKE_CXX_COMPILER="$CLANG_PATH/clang++" \
  ../..

# Build the project
ninja

# Run the program (adjust if the binary is named differently)
./EchoScript
