// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>

struct Stream {};

int main() {
  Stream* stream = 0;
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, stream);
}
