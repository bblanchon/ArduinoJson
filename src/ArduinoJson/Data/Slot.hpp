// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Memory/AllocableInMemoryPool.hpp"
#include "JsonVariantData.hpp"

namespace ArduinoJson {
namespace Internals {

struct Slot : AllocableInMemoryPool {
  JsonVariantData value;
  struct Slot* next;
  struct Slot* prev;
  const char* key;
};

}  // namespace Internals
}  // namespace ArduinoJson
