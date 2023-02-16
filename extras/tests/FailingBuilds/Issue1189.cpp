// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>

// a function should not be able to get a JsonDocument by value
void f(JsonDocument) {}

int main() {
  DynamicJsonDocument doc(1024);
  f(doc);
}
