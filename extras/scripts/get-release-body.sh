#!/bin/bash

set -eu

TAG="$1"
CHANGELOG="$2"

cat << END
## Changes

$(awk '/\* /{ FOUND=1; print; next } { if (FOUND) exit}' "$CHANGELOG")

[View version history](https://github.com/bblanchon/ArduinoJson/blob/$TAG/CHANGELOG.md)
END
