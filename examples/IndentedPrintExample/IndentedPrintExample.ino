/*
 * Arduino JSON library - IndentedPrint example
 * Benoit Blanchon 2014 - MIT License
 */

#include <ArduinoJson.h>

using namespace ArduinoJson::Internals;

void setup() {
  Serial.begin(9600);

  IndentedPrint serial(Serial);
  serial.setTabSize(4);

  serial.println("This is at indentation 0");
  serial.indent();
  serial.println("This is at indentation 1");
  serial.println("This is also at indentation 1");
  serial.indent();
  serial.println("This is at indentation 2");

  serial.unindent();
  serial.unindent();
  serial.println("This is back at indentation 0");
}

void loop() {
  // not used in this example
}