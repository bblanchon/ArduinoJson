#!/usr/bin/env bash

set -eu

SOURCE_DIR="$(dirname "$0")/../.."
WORK_DIR=$(mktemp -d)
trap 'rm -rf "$WORK_DIR"' EXIT

cp "$SOURCE_DIR/README.md" "$WORK_DIR/README.md"
cp "$SOURCE_DIR/CHANGELOG.md" "$WORK_DIR/CHANGELOG.md"
cp "$SOURCE_DIR/library.properties" "$WORK_DIR/library.properties"
cp "$SOURCE_DIR/LICENSE.txt" "$WORK_DIR/LICENSE.txt"
cp -r "$SOURCE_DIR/src" "$WORK_DIR/"
cp -r "$SOURCE_DIR/examples" "$WORK_DIR/"

cd "$WORK_DIR"
particle library upload -v
particle library publish -v
