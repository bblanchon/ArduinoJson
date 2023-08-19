#!/bin/bash

set -eu

VERSION="$1"
CHANGELOG="$2"
ARDUINOJSON_H="$3"

cat << END
---
branch: v7
version: $VERSION
date: '$(date +'%Y-%m-%d')'
$(extras/scripts/wandbox/publish.sh "$ARDUINOJSON_H")
---

$(awk '/\* /{ FOUND=1; print; next } { if (FOUND) exit}' "$CHANGELOG")
END
