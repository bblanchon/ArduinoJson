#!/bin/sh -eux

URL=https://cmake.org/files/v3.4/cmake-3.4.3-Linux-x86_64.tar.gz

curl -sS $URL | tar xz -C /tmp --strip 1

/tmp/bin/cmake -DCMAKE_CXX_COMPILER=$CMAKE_CXX_COMPILER .
make 
make test
