// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "MsgPack/MsgPackDeserializer.hpp"

namespace ArduinoJson {
// MsgPackError deserializeMsgPack(TDestination& destination, TString json);
// TDestination = JsonArray, JsonObject, JsonVariant
// TString = const std::string&, const String&
template <typename TDestination, typename TString>
typename Internals::EnableIf<!Internals::IsArray<TString>::value,
                             MsgPackError>::type
deserializeMsgPack(TDestination &destination, const TString &json,
                   uint8_t nestingLimit = ARDUINOJSON_DEFAULT_NESTING_LIMIT) {
  destination.clear();
  return Internals::makeMsgPackDeserializer(&destination.buffer(), json,
                                            nestingLimit)
      .parse(destination);
}
//
// MsgPackError deserializeMsgPack(TDestination& destination, TString json);
// TDestination = JsonArray, JsonObject, JsonVariant
// TString = const char*, const char[N], const FlashStringHelper*
template <typename TDestination, typename TString>
MsgPackError deserializeMsgPack(
    TDestination &destination, TString *json,
    uint8_t nestingLimit = ARDUINOJSON_DEFAULT_NESTING_LIMIT) {
  destination.clear();
  return Internals::makeMsgPackDeserializer(&destination.buffer(), json,
                                            nestingLimit)
      .parse(destination);
}
//
// MsgPackError deserializeMsgPack(TDestination& destination, TString json);
// TDestination = JsonArray, JsonObject, JsonVariant
// TString = std::istream&, Stream&
template <typename TDestination, typename TString>
MsgPackError deserializeMsgPack(
    TDestination &destination, TString &json,
    uint8_t nestingLimit = ARDUINOJSON_DEFAULT_NESTING_LIMIT) {
  destination.clear();
  return Internals::makeMsgPackDeserializer(&destination.buffer(), json,
                                            nestingLimit)
      .parse(destination);
}
}  // namespace ArduinoJson
