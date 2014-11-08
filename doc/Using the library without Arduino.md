Using the library without Arduino
=================================

This library is primarily design to be used with the Arduino IDE and therefore has a simplified setup procedure for that purpose.
If you use the Arduino IDE, please read [Using the library with Arduino](Using the library with Arduino.md).

However, it can be used without Arduino IDE with very little effort.

## Compiling the library

Step 1: Download source code:

    git clone https://github.com/bblanchon/ArduinoJson.git

Step 2: Generate the `Makefile` for your environment

    cd ArduinoJson
    cmake .

Step 3: Build

	make

## File paths

Assuming you installed the library into `<arduino-json>`, you need to add:

1. `<arduino-json>/include` to your include path
2. `<arduino-json>/lib` to your library path

## Headers

The following headers are required:

    #include <ArduinoJson/JsonArray.hpp>
    #include <ArduinoJson/JsonObject.hpp>
    #include <ArduinoJson/StaticJsonBuffer.hpp>

## Namespace

Every class of the library is declared in the `ArduinoJson` namespace, so you may want to add the following line after the `#include` statements:

    using namespace ArduinoJson;

----------

You are now ready to follow the instructions:

1. [Parsing JSON](Parsin JSON.md)
2. [Generating JSON](Generating JSON.md)