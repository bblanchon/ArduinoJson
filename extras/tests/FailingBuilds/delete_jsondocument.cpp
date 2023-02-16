// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>

struct Stream {};

int main() {
  JsonDocument* doc = new DynamicJsonDocument(42);
  delete doc;
}
