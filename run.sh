#!/usr/bin/env bash

set -e

BUILD_DIR="build"
BIN_PATH="$BUILD_DIR/src/trading_engine"

echo "================================="
echo " Trading Engine Runner"
echo "================================="

if [ ! -f "$BIN_PATH" ]; then
    echo "Binary not found. Building..."
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR
    cmake ..
    make -j
    cd ..
fi

case "$1" in

run)
    echo "Running Trading Engine..."
    $BIN_PATH
    ;;

test)
    echo "Running Unit Tests..."
    cd $BUILD_DIR
    ctest 
    ;;

match)
    echo "Running Matching Engine Functional Test..."
    cd $BUILD_DIR
    ctest -V -R MatchingTest
    ;;

build)
    echo "Building..."
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR
    cmake ..
    make -j
    ;;
clean)
    echo "Cleaning build..."
    rm -rf build
    ;;
*)

    echo "Usage:"
    echo "  ./run.sh run     -> run engine"
    echo "  ./run.sh test    -> run all tests"
    echo "  ./run.sh match   -> run matching test"
    echo "  ./run.sh build   -> build project"
    echo "  ./run.sh clean   -> clean build"
    ;;
esac