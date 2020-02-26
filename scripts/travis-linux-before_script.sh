#!/bin/bash

set -e
set -x

# Make sure everything is clean
rm -rf build
rm -rf install

# Create build directories
mkdir -p build/debug
mkdir -p build/release

# Prepare build environment (cmake)
/usr/bin/cmake --version

pushd build/debug
/usr/bin/cmake \
    -DCMAKE_INSTALL_PREFIX:PATH=$(pwd)/../../install/debug \
    -DCMAKE_BUILD_TYPE=Debug \
    -DINSTALL_GTEST=False \
    ../..
popd

pushd build/release
/usr/bin/cmake \
    -DCMAKE_INSTALL_PREFIX:PATH=$(pwd)/../../install/release \
    -DCMAKE_BUILD_TYPE=Release \
    -DINSTALL_GTEST=False \
    ../..
popd
