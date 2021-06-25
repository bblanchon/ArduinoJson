#!/bin/sh

###############
# BUILD
###############

(
cd extras/ci/espidf || exit

# set env variables
. ./esp-idf/export.sh

(
  # build all examples
  for EXAMPLE in ./components/ArduinoJson/examples/*/*.ino; do
    cp $EXAMPLE main/main.cpp
    idf.py build || exit
  done   
)
)

