#!/bin/sh

###############
# INSTALLATION
###############

# change into root template directory
cd extras/ci/espidf || exit

echo "installing esp-idf..."
if [ ! -d "esp-idf" ] ; then
  git clone https://github.com/espressif/esp-idf.git
fi
(
  cd esp-idf || exit
  git checkout 1d7068e4b
  git submodule update --init --recursive
  ./install.sh
)

echo "esp-idf installed"

if [ ! -d "components" ] ; then
    mkdir -p components
fi

echo "installing arduino-esp32"
(
  cd components || exit
  if [ ! -d "arduino" ] ; then
    git clone https://github.com/espressif/arduino-esp32.git arduino
  fi
  (
    cd arduino || exit
    git checkout master #21947eb
    git submodule update --init --recursive
  )
)
echo "arduino-esp32 installed"

echo "installing ArduinoJson"
(
  cd components || exit
  if [ ! -d "ArduinoJson" ] ; then
    git clone https://github.com/qt1/ArduinoJson.git 
    # TODO: must be switched (after PR merge) to: https://github.com/bblanchon/ArduinoJson.git
  fi
  (
    cd ArduinoJson || exit
    git checkout master
    git submodule update --init --recursive
  )
)
echo "ArduinoJson installed"

echo "installing Ethernet"
(
  cd components || exit
  if [ ! -d "Ethernet" ] ; then
    git clone https://github.com/andreaskuster/Ethernet.git
    # TODO: must be switched to (after PR merge): https://github.com/arduino-libraries/Ethernet.git
  fi
  (
    cd Ethernet || exit
    git checkout master
    git submodule update --init --recursive
  )
)
echo "Ethernet installed"

# set env variables
. ./esp-idf/export.sh

cd ../../..

