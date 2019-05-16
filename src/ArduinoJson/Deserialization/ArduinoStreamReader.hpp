// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#if ARDUINOJSON_ENABLE_ARDUINO_STREAM

#include <Stream.h>

namespace ARDUINOJSON_NAMESPACE {

struct ArduinoStreamReader {
  Stream& _stream;

 public:
  explicit ArduinoStreamReader(Stream& stream) : _stream(stream) {}

  int read() {
    // don't use _stream.read() as it ignores the timeout
    uint8_t c;
    return _stream.readBytes(&c, 1) ? c : -1;
  }
};

inline ArduinoStreamReader makeReader(Stream& input) {
  return ArduinoStreamReader(input);
}
}  // namespace ARDUINOJSON_NAMESPACE

#endif
