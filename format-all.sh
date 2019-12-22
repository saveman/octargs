#!/bin/bash

find include -name "*.hpp" -exec clang-format --verbose -i {} \;

find tests -name "*.hpp" -exec clang-format --verbose -i {} \;
find tests -name "*.cpp" -exec clang-format --verbose -i {} \;
