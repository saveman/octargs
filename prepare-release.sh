#!/bin/bash

# Usage:
# prepare-release.sh <version>
#
# where version = major.minor.patch

set -e

check_version_part ()
{
    local VERSION_PART=$1
    if [ "${VERSION_PART}" -eq "${VERSION_PART}" ] 2>/dev/null ; then
        if [ "${VERSION_PART}" -ge "0" ] ; then
            return 0
        fi
    fi
    return 1
}

recreate_release_dir ()
{
    rm -rf ${RELEASE_DIR}
    mkdir -p ${RELEASE_DIR}
    mkdir -p ${BUILD_DIR}
    mkdir -p ${INSTALL_DIR}
    mkdir -p ${PACKAGE_DIR}
}

rebuild_and_test ()
{
    pushd ${BUILD_DIR}
    cmake \
        -DCMAKE_BUILD_TYPE=Debug \
        -DRELEASE_VERSION=${RELEASE_VERSION} \
        -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
        -DCPACK_PACKAGE_DIRECTORY=${PACKAGE_DIR} \
        -DBUILD_TESTS=True \
        -DBUILD_EXAMPLES=False \
        -DENABLE_COVERAGE=False \
        ${SOURCE_DIR}
    cmake --build . -- all -j ${CPUCOUNT}
    cmake --build . -- test
    popd
}

rebuild_and_package ()
{
    local CPUCOUNT=$(nproc)
    pushd ${BUILD_DIR}
    cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -DRELEASE_VERSION=${RELEASE_VERSION} \
        -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
        -DCPACK_PACKAGE_DIRECTORY=${PACKAGE_DIR} \
        -DCMAKE_BUILD_TYPE=Debug \
        -DBUILD_TESTS=False \
        -DBUILD_EXAMPLES=False \
        -DENABLE_COVERAGE=False \
        ${SOURCE_DIR}
    cmake --build . -- all -j ${CPUCOUNT}
    cmake --build . -- install
    cmake --build . -- package
    cmake --build . -- package_source
    popd
}

check_git_branch ()
{
    local CURRENT_BRANCH=$(git rev-parse --symbolic-full-name --abbrev-ref HEAD)

    if [ "${RELEASE_BRANCH}" != "${CURRENT_BRANCH}" ]; then
        echo "Invalid branch: must be ${RELEASE_BRANCH}"
        if [ ${DRY_RUN} -eq 0 ]; then
            return 1
        fi
    fi
    return 0
}

check_git_tag ()
{
    if [ $(git tag -l "${RELEASE_TAG}") ]; then
        echo "Release tag already exists: ${RELEASE_TAG}"
        if [ ${DRY_RUN} -eq 0 ]; then
            return 1
        fi
    fi
    return 0
}

check_git_changes ()
{
    if [ "$(git status --porcelain --untracked-files=all)" != "" ]; then
        echo "There are uncommited changes"
        if [ ${DRY_RUN} -eq 0 ]; then
            return 1
        fi
    fi
    return 0
}

DRY_RUN=0
if [ "$1" == "--dry-run" ]; then
    DRY_RUN=1
    shift
fi
VERSION=$1

IFS="." read -ra VERSION_ARRAY <<< "${VERSION}"
VERSION_MAJOR=${VERSION_ARRAY[0]}
VERSION_MINOR=${VERSION_ARRAY[1]}
VERSION_PATCH=${VERSION_ARRAY[2]}

check_version_part "${VERSION_MAJOR}"
check_version_part "${VERSION_MINOR}"
check_version_part "${VERSION_PATCH}"

RELEASE_VERSION="${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
RELEASE_BRANCH="octargs-${VERSION_MAJOR}.${VERSION_MINOR}"
RELEASE_TAG="v${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"

echo "--------"
echo "Preparing version: ${RELEASE_VERSION}"
echo "--------"

check_git_branch
check_git_tag
check_git_changes

SOURCE_DIR=$(pwd)
RELEASE_DIR=$(pwd)/_release
BUILD_DIR=${RELEASE_DIR}/build
INSTALL_DIR=${RELEASE_DIR}/install
PACKAGE_DIR=${RELEASE_DIR}/package

recreate_release_dir
rebuild_and_test

recreate_release_dir
rebuild_and_package

if [ ${DRY_RUN} -eq 0 ]; then
    echo "Creating and pushing tag: ${RELEASE_TAG}"
    git tag "${RELEASE_TAG}"
    echo "Now if everything is correct push the tag manually:"
    echo "git push origin "${RELEASE_TAG}""
fi
