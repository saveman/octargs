#!/bin/bash

set -e
set -x

pushd build/debug
/usr/bin/cmake --build .
/usr/bin/cmake --build . -- install
/usr/bin/cmake --build . -- test
popd

pushd build/release
/usr/bin/cmake --build .
/usr/bin/cmake --build . -- install
/usr/bin/cmake --build . -- test
popd
