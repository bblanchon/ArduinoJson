#!/bin/sh -ex

BOARD=$1

cd "$(dirname "$0")/../../"

cp extras/particle/src/smocktest.ino src/
cp extras/particle/project.properties ./

particle compile "$BOARD"
