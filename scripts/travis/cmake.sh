#!/bin/sh -ex

if [ -n "$GCC" ]; then
	export CC="gcc-$GCC"
	export CXX="g++-$GCC"
fi

if [ -n "$CLANG" ]; then
	export CC="clang-$CLANG"
	export CXX="clang++-$CLANG"
fi

if [ -n "$SANITIZE" ]; then
	export CXXFLAGS="-fsanitize=$SANITIZE"
	BUILD_TYPE="Debug"
else
	BUILD_TYPE="Release"
fi

cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE .
cmake --build .
ctest --output-on-failure .
