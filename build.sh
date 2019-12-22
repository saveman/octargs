#!/bin/bash

CURRENT_DIR=$(pwd)

SOURCE_DIR=${CURRENT_DIR}
BUILD_DIR=${CURRENT_DIR}/build
INSTALL_DIR=${CURRENT_DIR}/install

set -x
set -e

rm -rf ${BUILD_DIR}
rm -rf ${INSTALL_DIR}

mkdir -p ${BUILD_DIR}

pushd ${BUILD_DIR}

cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} -DCMAKE_BUILD_TYPE=Debug ${SOURCE_DIR}
cmake --build .
cmake --build . -- install

popd

ls -l ${INSTALL_DIR}
