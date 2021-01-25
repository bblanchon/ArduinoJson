// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>

// a function should not be able to get a JsonDocument by value
void f(JsonDocument) {}

int main() {
  DynamicJsonDocument doc(1024);
  f(doc);
}
