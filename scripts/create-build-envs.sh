#!/bin/bash

export PATH="$PATH:/Applications/CMake.app/Contents/bin/"

cd $(dirname $0)/..
ROOT=$(pwd)

mkdir "build"
cd build
BUILD=$(pwd)

build-env()
{
	cd $BUILD
	mkdir "$1"
	cd "$1"
	cmake "$ROOT" -G "$2"
}

build-env "Xcode" "Xcode"
build-env "Sublime" "Sublime Text 2 - Unix Makefiles"
build-env "Make" "Unix Makefiles"
