// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Data/JsonVariantData.hpp"

namespace ARDUINOJSON_NAMESPACE {

struct VariantSlot {
  JsonVariantData value;
  struct VariantSlot* next;
  struct VariantSlot* prev;
  union {
    const char* linkedKey;
    StringSlot* ownedKey;
  };
};

}  // namespace ARDUINOJSON_NAMESPACE
