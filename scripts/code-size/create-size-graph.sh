#!/bin/bash

set -eu

PATH="$PATH:/c/Program Files (x86)/Arduino/"
OUTPUT="$(pwd)/sizes.csv"
BOARD="arduino:avr:uno"

compile() {(
  cd "$(dirname "$1")"
  arduino --verify --board $BOARD "$(basename "$1")" 2>/dev/null | grep -e 'Sketch uses' | sed -E 's/.*uses ([0-9]*),?([0-9]+).*/\1\2/'
)}

cd "$(dirname $0)/../.."

measure_version () {
  VERSION=$(git describe)
  DATE=$(git log -1 --date=short --pretty=format:%cd)
  PARSER_SIZE=$(compile examples/JsonParserExample/JsonParserExample.ino)
  GENERATOR_SIZE=$(compile examples/JsonGeneratorExample/JsonGeneratorExample.ino)

  echo "$VERSION,$DATE,$PARSER_SIZE,$GENERATOR_SIZE" | tee -a "$OUTPUT"
}

echo "Commit,Date,JsonParserExample,JsonGeneratorExample" | tee "$OUTPUT"
measure_version
while git checkout -q HEAD~1
do
	measure_version
done
