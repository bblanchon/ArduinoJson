#!/bin/sh -eux

pip install --user platformio

rm -r test

for EXAMPLE in JsonParserExample JsonGeneratorExample
do
	platformio ci examples/$EXAMPLE/$EXAMPLE.ino -l '.' -b $BOARD
done
