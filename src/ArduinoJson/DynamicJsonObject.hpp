// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonObject.hpp"
#include "Memory/DynamicJsonBuffer.hpp"

namespace ArduinoJson {
class DynamicJsonObject : public JsonObject {
  DynamicJsonBuffer _buffer;

 public:
  DynamicJsonObject() : JsonObject(&_buffer) {}
  DynamicJsonObject(size_t capacity)
      : JsonObject(&_buffer), _buffer(capacity - sizeof(JsonObject)) {}

  DynamicJsonBuffer& buffer() {
    return _buffer;
  }

  size_t memoryUsage() const {
    return _buffer.size() + sizeof(JsonObject);
  }
};
}  // namespace ArduinoJson
