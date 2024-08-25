// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Polyfills/limits.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

struct StringNode;

struct FreeSlot {
  SlotId next;
};

union VariantSlot {
  VariantSlot() {}

  VariantData variant;
  FreeSlot free;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
