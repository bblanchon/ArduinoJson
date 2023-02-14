#!/bin/bash

set -e

RE_RELATIVE_INCLUDE='^#[[:space:]]*include[[:space:]]*"(.*)"'
RE_ABSOLUTE_INCLUDE='^#[[:space:]]*include[[:space:]]*<(ArduinoJson/.*)>'
RE_SYSTEM_INCLUDE='^#[[:space:]]*include[[:space:]]*<(.*)>'
RE_EMPTY='^(#[[:space:]]*pragma[[:space:]]+once)?[[:space:]]*(//.*)?$'
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
	perl -p0i -e 's|ARDUINOJSON_END_PUBLIC_NAMESPACE\r?\nARDUINOJSON_BEGIN_PUBLIC_NAMESPACE\r?\n||igs' "$1"
	perl -p0i -e 's|ARDUINOJSON_END_PRIVATE_NAMESPACE\r?\nARDUINOJSON_BEGIN_PRIVATE_NAMESPACE\r?\n||igs' "$1"
	rm -f "$1.bak"
}

INCLUDED=()
INPUT=$1
OUTPUT=$2
process "$INPUT" true > "$OUTPUT"
simplify_namespaces "$OUTPUT"
