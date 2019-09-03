#!/bin/sh -ex

export CC="$_CC"
export CXX="$_CXX"

if [ -n "$SANITIZE" ]; then
	export CXXFLAGS="-fsanitize=$SANITIZE"
	BUILD_TYPE="Debug"
else
	BUILD_TYPE="Release"
fi

cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE .
cmake --build .
