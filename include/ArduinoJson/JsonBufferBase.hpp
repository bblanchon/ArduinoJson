// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "Deserialization/JsonParser.hpp"

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#elif defined(__GNUC__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#endif
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

namespace ArduinoJson {
template <typename TDerived>
class JsonBufferBase : public JsonBuffer {
 public:
  // Allocates and populate a JsonArray from a JSON string.
  //
  // The First argument is a pointer to the JSON string, the memory must be
  // writable
  // because the parser will insert null-terminators and replace escaped chars.
  //
  // The second argument set the nesting limit
  //
  // Returns a reference to the new JsonObject or JsonObject::invalid() if the
  // allocation fails.
  // With this overload, the JsonBuffer will make a copy of the string
  template <typename TString>
  JsonArray &parseArray(
      const TString &json,
      uint8_t nestingLimit = ARDUINOJSON_DEFAULT_NESTING_LIMIT) {
    return Internals::makeParser(that(), json, nestingLimit).parseArray();
  }
  template <typename TString>
  JsonArray &parseArray(
      TString &json, uint8_t nestingLimit = ARDUINOJSON_DEFAULT_NESTING_LIMIT) {
    return Internals::makeParser(that(), json, nestingLimit).parseArray();
  }

  // Allocates and populate a JsonObject from a JSON string.
  //
  // The First argument is a pointer to the JSON string, the memory must be
  // writable
  // because the parser will insert null-terminators and replace escaped chars.
  //
  // The second argument set the nesting limit
  //
  // Returns a reference to the new JsonObject or JsonObject::invalid() if the
  // allocation fails.
  template <typename TString>
  JsonObject &parseObject(
      const TString &json,
      uint8_t nestingLimit = ARDUINOJSON_DEFAULT_NESTING_LIMIT) {
    return Internals::makeParser(that(), json, nestingLimit).parseObject();
  }
  template <typename TString>
  JsonObject &parseObject(
      TString &json, uint8_t nestingLimit = ARDUINOJSON_DEFAULT_NESTING_LIMIT) {
    return Internals::makeParser(that(), json, nestingLimit).parseObject();
  }

  // Generalized version of parseArray() and parseObject(), also works for
  // integral types.
  template <typename TString>
  JsonVariant parse(const TString &json,
                    uint8_t nestingLimit = ARDUINOJSON_DEFAULT_NESTING_LIMIT) {
    return Internals::makeParser(that(), json, nestingLimit).parseVariant();
  }
  template <typename TString>
  JsonVariant parse(TString &json,
                    uint8_t nestingLimit = ARDUINOJSON_DEFAULT_NESTING_LIMIT) {
    return Internals::makeParser(that(), json, nestingLimit).parseVariant();
  }

 private:
  TDerived *that() {
    return static_cast<TDerived *>(this);
  }
};
}

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif
#endif
