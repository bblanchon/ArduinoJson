// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>

// See issue #2135

int main() {
  JsonObject obj;
  deserializeJson(obj, "");
}
