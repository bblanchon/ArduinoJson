// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>

extern "C" void app_main() {
  char buffer[256];
  JsonDocument doc;

  doc["hello"] = "world";
  serializeJson(doc, buffer);
  deserializeJson(doc, buffer);
  serializeMsgPack(doc, buffer);
  deserializeMsgPack(doc, buffer);
}
