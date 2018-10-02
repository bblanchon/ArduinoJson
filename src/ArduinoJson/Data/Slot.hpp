// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Memory/AllocableInMemoryPool.hpp"
#include "JsonVariantData.hpp"

namespace ARDUINOJSON_NAMESPACE {

struct Slot : AllocableInMemoryPool {
  JsonVariantData value;
  struct Slot* next;
  struct Slot* prev;
  const char* key;
};

}  // namespace ARDUINOJSON_NAMESPACE
