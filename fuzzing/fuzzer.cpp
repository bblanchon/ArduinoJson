#include <ArduinoJson.h>

class memstream : public std::istream {
  struct membuf : std::streambuf {
    membuf(const uint8_t *p, size_t l) {
      setg((char *)p, (char *)p, (char *)p + l);
    }
  };
  membuf _buffer;

 public:
  memstream(const uint8_t *p, size_t l)
      : std::istream(&_buffer), _buffer(p, l) {
    rdbuf(&_buffer);
  }
};

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  DynamicJsonVariant variant;
  memstream json(data, size);
  JsonError error = deserializeJson(variant, json);
  JsonVariant variant = jsonBuffer.parse(json);
  if (!error) {
    variant.as<std::string>();  // <- serialize to JSON
  }
  return 0;
}
