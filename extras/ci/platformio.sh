#!/bin/sh -eux

BOARD=$1

case $BOARD in
uno)
  platformio lib install 868 # SD library
  platformio lib install 872 # Ethernet library
  ;;
esp01)
  platformio lib uninstall 161 || true
  platformio lib uninstall 868 || true
  platformio lib uninstall 872 || true
  ;;
esac

for EXAMPLE in $PWD/examples/*/*.ino; 
do
	platformio ci $EXAMPLE -l '.' -b $BOARD
done
