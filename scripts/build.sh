#!/bin/bash

# Build script for Void Contingency
# Usage: ./build.sh [Debug|Release|RelWithDebInfo|MinSizeRel]

# Default build type
BUILD_TYPE=${1:-Debug}

# Validate build type
if [[ ! "$BUILD_TYPE" =~ ^(Debug|Release|RelWithDebInfo|MinSizeRel)$ ]]; then
    echo "Error: Invalid build type. Must be one of: Debug, Release, RelWithDebInfo, MinSizeRel"
    exit 1
fi

# Get the project root directory (one level up from the scripts directory)
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"

# Function to check if a command exists
check_command() {
    if ! command -v "$1" &> /dev/null; then
        echo "Error: $1 is not installed or not in PATH"
        exit 1
    fi
}

# Check prerequisites
echo "Checking prerequisites..."
check_command "cmake"

# Create build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir -p "$BUILD_DIR"
fi

# Navigate to build directory
cd "$BUILD_DIR" || exit 1

# Configure with CMake
echo "Configuring project with CMake ($BUILD_TYPE)..."
cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" "$PROJECT_ROOT"
if [ $? -ne 0 ]; then
    echo "Error: CMake configuration failed"
    exit 1
fi

# Build the project
echo "Building project..."
cmake --build . --config "$BUILD_TYPE"
if [ $? -ne 0 ]; then
    echo "Error: Build failed"
    exit 1
fi

echo -e "\nBuild completed successfully!"
echo "Executable location: $BUILD_DIR/bin/$BUILD_TYPE/VoidContingency"

# Ask if user wants to run tests
read -p "Would you like to run the tests? (y/n) " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "Running tests..."
    ctest -C "$BUILD_TYPE"
fi 