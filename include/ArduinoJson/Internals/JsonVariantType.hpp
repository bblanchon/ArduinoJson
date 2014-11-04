// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

namespace ArduinoJson {
namespace Internals {

enum JsonVariantType {
  JSON_UNDEFINED,
  JSON_INVALID,
  JSON_ARRAY,
  JSON_OBJECT,
  JSON_BOOLEAN,
  JSON_STRING,
  JSON_LONG,
  JSON_DOUBLE_0_DECIMALS
  // JSON_DOUBLE_1_DECIMAL
  // JSON_DOUBLE_2_DECIMALS
  // etc.
};
}
}
