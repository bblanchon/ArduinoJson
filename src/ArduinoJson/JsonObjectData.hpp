// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/List.hpp"
#include "Data/ValueSaver.hpp"
#include "JsonPair.hpp"
#include "Memory/JsonBufferAllocated.hpp"
#include "Polyfills/type_traits.hpp"

// Returns the size (in bytes) of an object with n elements.
// Can be very handy to determine the size of a StaticJsonBuffer.
#define JSON_OBJECT_SIZE(NUMBER_OF_ELEMENTS)        \
  (sizeof(ArduinoJson::Internals::JsonObjectData) + \
   (NUMBER_OF_ELEMENTS) *                           \
       sizeof(ArduinoJson::Internals::JsonObjectData::node_type))

namespace ArduinoJson {
namespace Internals {
struct JsonObjectData : List<JsonPair>, JsonBufferAllocated {
  explicit JsonObjectData(JsonBuffer* buf) throw() : List<JsonPair>(buf) {}
};
}  // namespace Internals
}  // namespace ArduinoJson
