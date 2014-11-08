Using the library without Arduino
=================================

This library is primarily design to be used with the Arduino IDE and therefore has a simplified setup procedure for that purpose.
If you don't use the Arduino IDE, please read [Using the library without Arduino](Using the library without Arduino.md).

## Install the library

[Download the zip package](https://github.com/bblanchon/ArduinoJson/releases) and extract it to:

    <your Arduino Sketch folder>/libraries/ArduinoJson

Then restart the Arduino IDE.

## Run the examples sketches

Click `File` / `Example` / `ArduinoJson`.

![Screen capture of Arduino IDE](http://i.imgur.com/g5UwkVh.png)


## Use the library in your sketches

Just add the following line at the top of your program:

    #include <ArduinoJson.h>

Then follow the instructions:

1. [Parsing JSON](Parsin JSON.md)
2. [Generating JSON](Generating JSON.md)