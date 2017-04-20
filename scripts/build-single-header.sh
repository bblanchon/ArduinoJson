#!/bin/bash

TAG=$(git describe)
RE_INCLUDE='^#include[[:space:]]*["<](.*)[">]'
RE_EMPTY='^(#pragma[[:space:]]+once)?[[:space:]]*(//.*)?$'

declare -A INCLUDED

process()
{
	local PARENT=$1
	local FOLDER=$(dirname $1)
	local SHOW_COMMENT=$2
	while IFS= read -r LINE; do
		if [[ $LINE =~ $RE_INCLUDE ]]; then
			local CHILD=${BASH_REMATCH[1]}
			pushd "$FOLDER" > /dev/null
			if [[ -e $CHILD ]]; then
				local CHILD_PATH=$(realpath $CHILD)
				if [[ ! ${INCLUDED[$CHILD_PATH]} ]]; then
					#echo "// $PARENT -> $CHILD"
					INCLUDED[$CHILD_PATH]=true
					process "$CHILD" false
				fi
			else
				if [[ ! ${INCLUDED[$CHILD]} ]]; then
					echo "$LINE"
					INCLUDED[$CHILD]=true
				fi
			fi
			popd > /dev/null
		elif [[ "${SHOW_COMMENT}" = "true" ]] ; then
			echo "$LINE"
		elif [[ ! $LINE =~ $RE_EMPTY ]]; then
			echo "$LINE"
		fi
	done < $PARENT
}

cd $(dirname $0)/../
INCLUDED=()
process src/ArduinoJson.h true > ../ArduinoJson-$TAG.h
INCLUDED=()
process src/ArduinoJson.hpp true > ../ArduinoJson-$TAG.hpp