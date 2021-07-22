[![GitHub License](https://img.shields.io/github/license/saveman/octargs.svg?style=flat)](https://github.com/saveman/octargs/blob/master/COPYING)
[![GitHub Release](https://img.shields.io/github/release/saveman/octargs.svg?style=flat)](https://github.com/saveman/octargs/releases/latest)
[![GitHub Release](https://img.shields.io/github/release/saveman/octargs.svg?style=flat&sort=semver)](https://github.com/saveman/octargs/releases)
[![Build Status](https://travis-ci.com/saveman/octargs.svg?branch=master)](https://travis-ci.org/saveman/octargs)
[![Coverity Scan](https://scan.coverity.com/projects/20102/badge.svg)](https://scan.coverity.com/projects/saveman-octargs)

# OCTARGS - Argument Parsing Library
OCTARGS is a C++ library for parsing command line arguments.

* Header-only library.
* Requires C++11.
* Released under MIT license.

## Features supported:

- Arguments without values (e.g. "--verbose").
- Arguments with values (e.g. "--level=9", "--jobs 9").
- Exclusive arguments (e.g. "--help").
- Positional arguments (e.g. "cat file1.txt file2.txt").
- Short and long names (e.g. "-j", "--jobs", "--parallel-jobs").
- Subparsers (e.g. "git add <params>", "git init <params>").
- Default values
- Limiting allowed values
- Converting values to a requested type (with checking)
- Storing converted values in object variables automatically (with checking and automatic type detection).
- Automatic usage documentation.
- Support for char and wchar_t character types (and more with a limited effort).

# Documentation

Project [documentation](https://saveman.github.io/octargs/) is generated using [Doxygen](http://www.doxygen.nl/) and hosted on GitHub pages.

# Examples

Short code snippets showing general concepts are available in the documentation. For a more complicated examples take a look into [Examples](examples/) folder.
