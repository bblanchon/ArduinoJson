// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>

// See issue #1498

int main() {
  DynamicJsonDocument doc(1024);
  doc["dummy"].as<char>();
}
