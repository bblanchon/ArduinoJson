#!/bin/sh -eux

export CC=clang
export CXX=clang++

curl -sS https://cmake.org/files/v3.4/cmake-3.4.0-Linux-x86_64.tar.gz | tar xz -C /tmp --strip 1

/tmp/bin/cmake .
make 
make test
