#!/bin/bash

set -eu

VERSION="$1"
CHANGELOG="$2"
FRONTMATTER="$3"

cat << END
---
branch: v6
version: $VERSION
date: '$(date +'%Y-%m-%d')'
$(cat "$FRONTMATTER")
---

$(awk '/\* /{ FOUND=1; print; next } { if (FOUND) exit}' "$CHANGELOG")
END
