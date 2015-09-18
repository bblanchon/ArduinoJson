#!/bin/bash

TAG=$(git describe)
OUTPUT="ArduinoJson-$TAG-old-layout.zip"

cd $(dirname $0)/..

cat > ArduinoJson.h <<END
// WARNING:
// --------
// This file is a workaround for old version of the Arduino IDE.
// If you are using Arduino IDE 1.0.6 or above, then you installed the wrong 
// package of ArduinoJson.
// In that case, just delete the current installation and install the package.

END
cp ArduinoJson.h ArduinoJson.cpp

cat "include/ArduinoJson.h" | sed 's!include "!include "include/!g' >> ArduinoJson.h

find src -name "*.cpp" |
while read FILE; do
	echo >> ArduinoJson.cpp
	echo "// $FILE" >> ArduinoJson.cpp
	echo "//" >> ArduinoJson.cpp
	cat "$FILE" | sed 's!\.\./!!g' >> ArduinoJson.cpp
done

unix2dos ArduinoJson.cpp
unix2dos ArduinoJson.h

pushd ..

# remove existing file
rm -f $OUTPUT

# create zipman dos2
7z a $OUTPUT \
	ArduinoJson/CHANGELOG.md \
	ArduinoJson/examples \
	ArduinoJson/include/ArduinoJson \
	ArduinoJson/keywords.txt \
	ArduinoJson/LICENSE.md \
	ArduinoJson/README.md \
	ArduinoJson/ArduinoJson.h \
	ArduinoJson/ArduinoJson.cpp \
	-x!ArduinoJson/src/CMakeLists.txt

popd

rm ArduinoJson.h
rm ArduinoJson.cpp
