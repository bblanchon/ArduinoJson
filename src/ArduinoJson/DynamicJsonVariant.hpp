// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonVariant.hpp"
#include "Memory/DynamicJsonBuffer.hpp"

namespace ArduinoJson {

class DynamicJsonVariant : public JsonVariant {
  DynamicJsonBuffer _buffer;

 public:
  DynamicJsonVariant() : JsonVariant() {}
  DynamicJsonVariant(size_t capacity) : JsonVariant(), _buffer(capacity) {}

  template <typename T>
  DynamicJsonVariant& operator=(const T& value) {
    _buffer.clear();
    JsonVariant::operator=(value);
    return *this;
  }

  template <typename T>
  DynamicJsonVariant& operator=(const T* value) {
    _buffer.clear();
    JsonVariant::operator=(value);
    return *this;
  }

  DynamicJsonBuffer& buffer() {
    return _buffer;
  }

  void clear() {
    _buffer.clear();
  }

  size_t memoryUsage() const {
    return _buffer.size();
  }
};
}  // namespace ArduinoJson
