// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Deserialization/JsonParser.hpp"

namespace ArduinoJson {
// bool deserializeJson(TDestination& destination, TString json);
// TDestination = JsonArray, JsonObject, JsonVariant
// TString = const std::string&, const String&
template <typename TDestination, typename TString>
typename Internals::EnableIf<!Internals::IsArray<TString>::value, bool>::type
deserializeJson(TDestination &destination, const TString &json,
                uint8_t nestingLimit = ARDUINOJSON_DEFAULT_NESTING_LIMIT) {
  return Internals::makeParser(&destination.buffer(), json, nestingLimit)
      .parse(destination);
}
//
// bool deserializeJson(TDestination& destination, TString json);
// TDestination = JsonArray, JsonObject, JsonVariant
// TString = const char*, const char[N], const FlashStringHelper*
template <typename TDestination, typename TString>
bool deserializeJson(TDestination &destination, TString *json,
                     uint8_t nestingLimit = ARDUINOJSON_DEFAULT_NESTING_LIMIT) {
  return Internals::makeParser(&destination.buffer(), json, nestingLimit)
      .parse(destination);
}
//
// bool deserializeJson(TDestination& destination, TString json);
// TDestination = JsonArray, JsonObject, JsonVariant
// TString = std::istream&, Stream&
template <typename TDestination, typename TString>
bool deserializeJson(TDestination &destination, TString &json,
                     uint8_t nestingLimit = ARDUINOJSON_DEFAULT_NESTING_LIMIT) {
  return Internals::makeParser(&destination.buffer(), json, nestingLimit)
      .parse(destination);
}
}  // namespace ArduinoJson
