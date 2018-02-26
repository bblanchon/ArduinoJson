// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonArray.hpp"
#include "StaticJsonBuffer.hpp"

namespace ArduinoJson {

template <size_t CAPACITY>
class StaticJsonArray : public JsonArray {
  StaticJsonBuffer<CAPACITY - sizeof(JsonArray)> _buffer;

 public:
  StaticJsonArray() : JsonArray(&_buffer) {}

  size_t memoryUsage() const {
    return _buffer.size() + sizeof(JsonArray);
  }

  Internals::StaticJsonBufferBase& buffer() {
    return _buffer;
  }
};
}  // namespace ArduinoJson
