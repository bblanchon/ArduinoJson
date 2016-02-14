// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "JsonObjectKey.hpp"
#include "JsonVariant.hpp"

namespace ArduinoJson {

// A key value pair for JsonObject.
struct JsonPair {
  const char* key;
  JsonVariant value;
};
}
