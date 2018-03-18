// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonVariant.hpp"
#include "Memory/StaticJsonBuffer.hpp"

namespace ArduinoJson {

template <size_t CAPACITY = sizeof(JsonVariant)>
class StaticJsonVariant : public JsonVariant {
  StaticJsonBuffer<CAPACITY> _buffer;

 public:
  template <typename T>
  StaticJsonVariant& operator=(const T& value) {
    _buffer.clear();
    JsonVariant::operator=(value);
    return *this;
  }

  template <typename T>
  StaticJsonVariant& operator=(const T* value) {
    _buffer.clear();
    JsonVariant::operator=(value);
    return *this;
  }

  Internals::StaticJsonBufferBase& buffer() {
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
