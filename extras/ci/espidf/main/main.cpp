// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>

extern "C" void app_main() {
  char buffer[256];
  StaticJsonDocument<200> doc;

  doc["hello"] = "world";
  serializeJson(doc, buffer);
  deserializeJson(doc, buffer);
  serializeMsgPack(doc, buffer);
  deserializeMsgPack(doc, buffer);
}
