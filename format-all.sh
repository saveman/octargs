#!/bin/bash

find examples \( -name "*.hpp" -or -name "*.cpp" \) -exec clang-format --verbose -i {} \;
find include  \( -name "*.hpp" -or -name "*.cpp" \) -exec clang-format --verbose -i {} \;
find tests    \( -name "*.hpp" -or -name "*.cpp" \) -exec clang-format --verbose -i {} \;
