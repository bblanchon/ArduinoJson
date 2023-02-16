// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

namespace DeserializationOption {
class NestingLimit {
 public:
  NestingLimit() : _value(ARDUINOJSON_DEFAULT_NESTING_LIMIT) {}
  explicit NestingLimit(uint8_t n) : _value(n) {}

  NestingLimit decrement() const {
    ARDUINOJSON_ASSERT(_value > 0);
    return NestingLimit(static_cast<uint8_t>(_value - 1));
  }

  bool reached() const {
    return _value == 0;
  }

 private:
  uint8_t _value;
};
}  // namespace DeserializationOption

ARDUINOJSON_END_PUBLIC_NAMESPACE
