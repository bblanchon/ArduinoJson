// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
class JsonArray;
class JsonObject;

namespace Internals {

// Enumerated type to know the current type of a JsonVariant.
// The value determines which member of JsonVariantContent is used.
enum JsonVariantType {
  JSON_UNDEFINED,  // the JsonVariant has not been initialized
  JSON_UNPARSED,   // the JsonVariant contains an unparsed string
  JSON_STRING,     // the JsonVariant stores a const char*
  JSON_BOOLEAN,    // the JsonVariant stores a bool
  JSON_INTEGER,    // the JsonVariant stores an integer
  JSON_ARRAY,      // the JsonVariant stores a pointer to a JsonArray
  JSON_OBJECT,     // the JsonVariant stores a pointer to a JsonObject

  // The following values are reserved for float values
  // Multiple values are used for double, depending on the number of decimal
  // digits that must be printed in the JSON output.
  // This little trick allow to save one extra member in JsonVariant
  JSON_FLOAT_0_DECIMALS
  // JSON_FLOAT_1_DECIMAL
  // JSON_FLOAT_2_DECIMALS
  // ...
};
}
}
