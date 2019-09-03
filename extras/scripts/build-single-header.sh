#!/bin/bash

set -e

TAG=$(git describe)
RE_RELATIVE_INCLUDE='^#include[[:space:]]*"(.*)"'
RE_ABSOLUTE_INCLUDE='^#include[[:space:]]*<(ArduinoJson/.*)>'
RE_SYSTEM_INCLUDE='^#include[[:space:]]*<(.*)>'
RE_EMPTY='^(#pragma[[:space:]]+once)?[[:space:]]*(//.*)?$'
SRC_DIRECTORY="$(realpath "$(dirname $0)/../../src")"


declare -A INCLUDED

process()
{
	local PARENT=$1
	local FOLDER=$(dirname $1)
	local SHOW_COMMENT=$2
	while IFS= read -r LINE; do
		if [[ $LINE =~ $RE_ABSOLUTE_INCLUDE ]]; then
			local CHILD=${BASH_REMATCH[1]}
			local CHILD_PATH
			CHILD_PATH=$(realpath "$SRC_DIRECTORY/$CHILD")
			echo "$PARENT -> $CHILD" >&2
			if [[ ! ${INCLUDED[$CHILD_PATH]} ]]; then
				INCLUDED[$CHILD_PATH]=true
				process "$CHILD" false
			fi
		elif [[ $LINE =~ $RE_RELATIVE_INCLUDE ]]; then
			local CHILD=${BASH_REMATCH[1]}
			pushd "$FOLDER" > /dev/null
			local CHILD_PATH
			CHILD_PATH=$(realpath "$CHILD")
			echo "$PARENT -> $CHILD" >&2
			if [[ ! ${INCLUDED[$CHILD_PATH]} ]]; then
				INCLUDED[$CHILD_PATH]=true
				process "$CHILD" false
			fi
			popd > /dev/null
		elif [[ $LINE =~ $RE_SYSTEM_INCLUDE ]]; then
			local CHILD=${BASH_REMATCH[1]}
			echo "$PARENT -> <$CHILD>" >&2
			if [[ ! ${INCLUDED[$CHILD]} ]]; then
				echo "#include <$CHILD>"
				INCLUDED[$CHILD]=true
			fi
		elif [[ "${SHOW_COMMENT}" = "true" ]] ; then
			echo "$LINE"
		elif [[ ! $LINE =~ $RE_EMPTY ]]; then
			echo "$LINE"
		fi
	done < $PARENT
}

simplify_namespaces() {
	perl -p0i -e 's|\}  // namespace ARDUINOJSON_NAMESPACE\r?\nnamespace ARDUINOJSON_NAMESPACE \{\r?\n||igs' "$1"
}

cd $(dirname $0)/../
INCLUDED=()
process src/ArduinoJson.h true > ../ArduinoJson-$TAG.h
simplify_namespaces ../ArduinoJson-$TAG.h
g++ -x c++ -c -o ../smoketest.o - <<END
#include "../ArduinoJson-$TAG.h"
int main() {
	StaticJsonDocument<300> doc;
	deserializeJson(doc, "{}");
}
END

INCLUDED=()
process src/ArduinoJson.hpp true > ../ArduinoJson-$TAG.hpp
simplify_namespaces ../ArduinoJson-$TAG.hpp
g++ -x c++ -c -o ../smoketest.o - <<END
#include "../ArduinoJson-$TAG.hpp"
int main() {
	ArduinoJson::StaticJsonDocument<300> doc;
	ArduinoJson::deserializeJson(doc, "{}");
}
END
