#!/usr/bin/env bash

set -eux

cd "$(dirname $0)/.."

VERSION="$1"
IFS=".-" read MAJOR MINOR REVISION EXTRA < <(echo "$VERSION")
TAG="v$VERSION"
UNDERLINE=$(printf -- '-%.0s' $(seq 1 ${#TAG}))

sed -i~ -bE "4s/HEAD/$TAG/; 5s/-+/$UNDERLINE/" CHANGELOG.md
rm CHANGELOG.md*~
sed -i~ -bE "s/\"version\":.*$/\"version\": \"$VERSION\",/" library.json
rm library.json*~

sed -i~ -bE "s/version=.*$/version=$VERSION/" library.properties
rm library.properties*~

sed -i~ -bE \
	-e "s/ARDUINOJSON_VERSION .*$/ARDUINOJSON_VERSION \"$VERSION\"/" \
	-e "s/ARDUINOJSON_VERSION_MAJOR .*$/ARDUINOJSON_VERSION_MAJOR $MAJOR/" \
	-e "s/ARDUINOJSON_VERSION_MINOR .*$/ARDUINOJSON_VERSION_MINOR $MINOR/" \
	-e "s/ARDUINOJSON_VERSION_REVISION .*$/ARDUINOJSON_VERSION_REVISION $REVISION/" \
	src/ArduinoJson/version.hpp
rm src/ArduinoJson/version.hpp*~
