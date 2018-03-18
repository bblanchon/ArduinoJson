// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonObject.hpp"
#include "Memory/StaticJsonBuffer.hpp"

namespace ArduinoJson {

template <size_t CAPACITY>
class StaticJsonObject : public JsonObject {
  StaticJsonBuffer<CAPACITY - sizeof(JsonObject)> _buffer;

 public:
  StaticJsonObject() : JsonObject(&_buffer) {}

  void clear() {
    Internals::List<JsonPair>::clear();
    _buffer.clear();
  }

  size_t memoryUsage() const {
    return _buffer.size() + sizeof(JsonObject);
  }

  Internals::StaticJsonBufferBase& buffer() {
    return _buffer;
  }
};
}  // namespace ArduinoJson
