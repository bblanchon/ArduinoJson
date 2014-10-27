// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "ArduinoJson/JsonBuffer.hpp"

#include "ArduinoJson/Internals/JsonParser.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

// TODO: what happens if alloc returns NULL
void* operator new(size_t size, ArduinoJson::JsonBuffer* buffer) {
  return _buffer->alloc(size);
}
