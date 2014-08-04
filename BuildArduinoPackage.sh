#!/bin/bash

ZIP="C:\Program Files\7-Zip\7z.exe"
OUTPUT="ArduinoJson.zip"

cd ..

INPUT=$(find ArduinoJson -regex ".*\.\(cpp\|h\|md\|txt\|ino\)$" -not -regex ".*Tests/.*")

rm -f $OUTPUT
"$ZIP" a $OUTPUT $INPUT