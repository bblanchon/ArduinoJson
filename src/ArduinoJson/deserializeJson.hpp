// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Json/Deserialization/JsonDeserializer.hpp"

namespace ArduinoJson {
// JsonError deserializeJson(TDocument& doc, TString json);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TString = const std::string&, const String&
template <typename TDocument, typename TString>
typename Internals::EnableIf<!Internals::IsArray<TString>::value,
                             JsonError>::type
deserializeJson(TDocument &doc, const TString &json,
                uint8_t nestingLimit = ARDUINOJSON_DEFAULT_NESTING_LIMIT) {
  return Internals::makeParser(&doc.buffer(), json, nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
//
// JsonError deserializeJson(TDocument& doc, TString json);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TString = const char*, const char[N], const FlashStringHelper*
template <typename TDocument, typename TString>
JsonError deserializeJson(
    TDocument &doc, TString *json,
    uint8_t nestingLimit = ARDUINOJSON_DEFAULT_NESTING_LIMIT) {
  return Internals::makeParser(&doc.buffer(), json, nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
//
// JsonError deserializeJson(TDocument& doc, TString json);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TString = std::istream&, Stream&
template <typename TDocument, typename TString>
JsonError deserializeJson(
    TDocument &doc, TString &json,
    uint8_t nestingLimit = ARDUINOJSON_DEFAULT_NESTING_LIMIT) {
  return Internals::makeParser(&doc.buffer(), json, nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
}  // namespace ArduinoJson
