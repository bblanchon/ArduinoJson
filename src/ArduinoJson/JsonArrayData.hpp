// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/List.hpp"
#include "Data/ValueSaver.hpp"
#include "JsonVariant.hpp"
#include "Memory/JsonBufferAllocated.hpp"
#include "Polyfills/type_traits.hpp"

// Returns the size (in bytes) of an array with n elements.
// Can be very handy to determine the size of a StaticJsonBuffer.
#define JSON_ARRAY_SIZE(NUMBER_OF_ELEMENTS)        \
  (sizeof(ArduinoJson::Internals::JsonArrayData) + \
   (NUMBER_OF_ELEMENTS) *                          \
       sizeof(ArduinoJson::Internals::JsonArrayData::node_type))

namespace ArduinoJson {
namespace Internals {
struct JsonArrayData : List<JsonVariant>, JsonBufferAllocated {
  explicit JsonArrayData(JsonBuffer *buf) throw() : List<JsonVariant>(buf) {}
};
}  // namespace Internals
}  // namespace ArduinoJson
