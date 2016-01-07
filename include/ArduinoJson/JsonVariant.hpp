// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include <stddef.h>
#include <stdint.h>  // for uint8_t

#include "Internals/JsonPrintable.hpp"
#include "Internals/JsonVariantContent.hpp"
#include "Internals/JsonVariantType.hpp"
#include "Internals/Unparsed.hpp"
#include "JsonVariantBase.hpp"

namespace ArduinoJson {

// Forward declarations.
class JsonArray;
class JsonObject;

// A variant that can be a any value serializable to a JSON value.
//
// It can be set to:
// - a boolean
// - a char, short, int or a long (signed or unsigned)
// - a string (const char*)
// - a reference to a JsonArray or JsonObject
class JsonVariant : public JsonVariantBase<JsonVariant> {
 public:
  // Creates an uninitialized JsonVariant
  FORCE_INLINE JsonVariant() : _type(Internals::JSON_UNDEFINED) {}

  // Create a JsonVariant containing a boolean value.
  // It will be serialized as "true" or "false" in JSON.
  FORCE_INLINE JsonVariant(bool value);

  // Create a JsonVariant containing a floating point value.
  // The second argument specifies the number of decimal digits to write in
  // the JSON string.
  FORCE_INLINE JsonVariant(float value, uint8_t decimals = 2);
  FORCE_INLINE JsonVariant(double value, uint8_t decimals = 2);

  // Create a JsonVariant containing an integer value.
  FORCE_INLINE JsonVariant(signed char value);
  FORCE_INLINE JsonVariant(signed long value);
  FORCE_INLINE JsonVariant(signed int value);
  FORCE_INLINE JsonVariant(signed short value);
  FORCE_INLINE JsonVariant(unsigned char value);
  FORCE_INLINE JsonVariant(unsigned long value);
  FORCE_INLINE JsonVariant(unsigned int value);
  FORCE_INLINE JsonVariant(unsigned short value);

  // Create a JsonVariant containing a string.
  FORCE_INLINE JsonVariant(const char *value);

  // Create a JsonVariant containing an unparsed string
  FORCE_INLINE JsonVariant(Internals::Unparsed value);

  // Create a JsonVariant containing a reference to an array.
  FORCE_INLINE JsonVariant(JsonArray &array);

  // Create a JsonVariant containing a reference to an object.
  FORCE_INLINE JsonVariant(JsonObject &object);

  // Get the variant as the specified type.
  // See cast operators for details.
  template <typename T>
  T as() const;

  // Tells weither the variant has the specified type.
  // Returns true if the variant has type type T, false otherwise.
  template <typename T>
  bool is() const;

  // Serialize the variant to a JsonWriter
  void writeTo(Internals::JsonWriter &writer) const;

  // TODO: rename
  template <typename T>
  static T invalid();

 private:
  Internals::JsonFloat asFloat() const;
  Internals::JsonInteger asInteger() const;

  // The current type of the variant
  Internals::JsonVariantType _type;

  // The various alternatives for the value of the variant.
  Internals::JsonVariantContent _content;
};

inline JsonVariant float_with_n_digits(float value, uint8_t digits) {
  return JsonVariant(value, digits);
}

inline JsonVariant double_with_n_digits(double value, uint8_t digits) {
  return JsonVariant(value, digits);
}
}

// Include inline implementations
#include "JsonVariant.ipp"
