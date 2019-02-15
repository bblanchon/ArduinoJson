// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

namespace ARDUINOJSON_NAMESPACE {

class DummyWriter {
 public:
  size_t write(uint8_t) {
    return 1;
  }

  size_t write(const uint8_t*, size_t n) {
    return n;
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
