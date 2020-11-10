#!/bin/sh -ex

cd "$(dirname "$0")/../../"

npm install -g particle-cli

particle login -t "${PARTICLE_TOKEN}"

cp extras/particle/src/smocktest.ino src/
cp extras/particle/project.properties ./

particle compile "$BOARD"
