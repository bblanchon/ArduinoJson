#!/usr/bin/env bash

set -eu

ARDUINOJSON_H="$1"

read_string() {
	jq --slurp --raw-input '.' "$1"
}

compile() {
	FILE_PATH="$(dirname $0)/$1.cpp"
	cat >parameters.json <<END
{
  "code":$(read_string "$FILE_PATH"),
  "codes": [{"file":"ArduinoJson.h","code":$(read_string "$ARDUINOJSON_H")}],
  "options": "warning,c++11",
  "compiler": "gcc-5.5.0",
  "save": true
}
END
	URL=$(curl -sS -H "Content-type: application/json" -d @parameters.json  https://wandbox.org/api/compile.json | jq --raw-output .url)
	rm parameters.json
	[ -n "$URL" ] && echo "$1: $URL"
}

compile "JsonGeneratorExample"
compile "JsonParserExample"
compile "MsgPackParserExample"
