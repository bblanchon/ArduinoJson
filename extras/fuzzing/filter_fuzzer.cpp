#include <ArduinoJson.h>

#include <cstring>

/* Fuzzes deserialization with a DeserializationOption::Filter, a feature
 * used on memory-constrained devices to parse only a subset of an
 * attacker-controlled JSON document. The filter document itself is also
 * attacker-influenced data here, mirroring setups where the filter is
 * built from configuration input.
 *
 * Input layout: <filter JSON> '\n' <payload JSON>.
 * Without a separator, the input is used for both. */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  const uint8_t* sep =
      static_cast<const uint8_t*>(memchr(data, '\n', size));

  JsonDocument filterDoc;
  const uint8_t* payload = data;
  size_t payloadLen = size;

  if (sep) {
    deserializeJson(filterDoc, data, sep - data);
    payload = sep + 1;
    payloadLen = size - (payload - data);
  } else {
    deserializeJson(filterDoc, data, size);
  }

  JsonDocument doc;
  DeserializationError error = deserializeJson(
      doc, payload, payloadLen, DeserializationOption::Filter(filterDoc));
  if (!error) {
    std::string json;
    serializeJson(doc, json);
  }
  return 0;
}
