// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonArray.hpp"
#include "Memory/DynamicJsonBuffer.hpp"

namespace ArduinoJson {
class DynamicJsonArray : public JsonArray {
  DynamicJsonBuffer _buffer;

 public:
  DynamicJsonArray() : JsonArray(&_buffer) {}
  DynamicJsonArray(size_t capacity)
      : JsonArray(&_buffer), _buffer(capacity - sizeof(JsonArray)) {}

  size_t memoryUsage() const {
    return _buffer.size() + sizeof(JsonArray);
  }

  DynamicJsonBuffer& buffer() {
    return _buffer;
  }
};
}  // namespace ArduinoJson
