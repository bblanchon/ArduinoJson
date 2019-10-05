#!/bin/sh -ex

"$(dirname "$0")/build.sh"
ctest --output-on-failure .
