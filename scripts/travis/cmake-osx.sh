#!/bin/sh -eux

URL=https://cmake.org/files/v3.4/cmake-3.4.3-Darwin-x86_64.tar.gz

curl -sS $URL | tar xz -C /tmp --strip 1

/tmp/CMake.app/Contents/bin/cmake .
make
make test
