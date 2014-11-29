Using the library without Arduino
=================================

This library is primarily design to be used with the Arduino IDE and therefore has a simplified setup procedure for that environment.
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


----------

You are now ready to follow the instructions:

1. [Parsing JSON](Parsin JSON.md)
2. [Generating JSON](Generating JSON.md)