// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#if ARDUINOJSON_ENABLE_ARDUINO_STREAM

#include <Stream.h>

namespace ARDUINOJSON_NAMESPACE {

struct ArduinoStreamReader {
  Stream& _stream;
  char _current;
  bool _ended;

 public:
  explicit ArduinoStreamReader(Stream& stream)
      : _stream(stream), _current(0), _ended(false) {}

  char read() {
    // don't use _stream.read() as it ignores the timeout
    char c = 0;
    _ended = _stream.readBytes(&c, 1) == 0;
    return c;
  }

  bool ended() const {
    return _ended;
  }
};

inline ArduinoStreamReader makeReader(Stream& input) {
  return ArduinoStreamReader(input);
}
}  // namespace ARDUINOJSON_NAMESPACE

#endif
