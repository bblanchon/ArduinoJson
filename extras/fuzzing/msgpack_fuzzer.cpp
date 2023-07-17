#include <ArduinoJson.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  JsonDocument doc;
  DeserializationError error = deserializeMsgPack(doc, data, size);
  if (!error) {
    std::string json;
    serializeMsgPack(doc, json);
  }
  return 0;
}
