// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class DummyWriter {
 public:
  size_t write(uint8_t) {
    return 1;
  }

  size_t write(const uint8_t*, size_t n) {
    return n;
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
