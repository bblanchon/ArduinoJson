#!/bin/bash

set -eu

INPUT=$1
OUTPUT=$2

cd "$INPUT"

# remove existing file
rm -f "$OUTPUT"

# create zip
7z a "$OUTPUT" \
	-xr!.vs \
	CHANGELOG.md \
	examples \
	src \
	keywords.txt \
	library.properties \
	LICENSE.md \
	README.md \
	ArduinoJson.h
