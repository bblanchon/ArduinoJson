// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {

namespace Internals {

// Enumerated type to know the current type of a JsonVariant.
// The value determines which member of JsonVariantContent is used.
enum JsonVariantType {
  JSON_NULL,
  JSON_LINKED_RAW,
  JSON_OWNED_RAW,
  JSON_LINKED_STRING,
  JSON_OWNED_STRING,
  JSON_BOOLEAN,
  JSON_POSITIVE_INTEGER,
  JSON_NEGATIVE_INTEGER,
  JSON_ARRAY,
  JSON_OBJECT,
  JSON_FLOAT
};
}  // namespace Internals
}  // namespace ArduinoJson
