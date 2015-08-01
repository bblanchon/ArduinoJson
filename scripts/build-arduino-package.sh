#!/bin/bash

TAG=$(git describe)
OUTPUT="ArduinoJson-$TAG.zip"

cd ../..

# remove existing file
rm -f $OUTPUT

# create zip
7z a $OUTPUT \
	ArduinoJson/CHANGELOG.md \
	ArduinoJson/examples \
	ArduinoJson/include \
	ArduinoJson/keywords.txt \
	ArduinoJson/LICENSE.md \
	ArduinoJson/README.md \
	ArduinoJson/src	\
	ArduinoJson/ArduinoJson.h \
	ArduinoJson/ArduinoJson.cpp	\
    -x!ArduinoJson/src/CMakeLists.txt