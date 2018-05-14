// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Json/Deserialization/JsonDeserializer.hpp"
#include "Reading/Reader.hpp"
#include "Writing/Writer.hpp"

namespace ArduinoJson {
// JsonError deserializeJson(TDocument& doc, TString input);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TString = const std::string&, const String&
template <typename TDocument, typename TString>
typename Internals::EnableIf<!Internals::IsArray<TString>::value,
                             JsonError>::type
deserializeJson(TDocument &doc, const TString &input) {
  using namespace Internals;
  return makeJsonDeserializer(&doc.buffer(), makeReader(input),
                              makeWriter(doc.buffer(), input), doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
//
// JsonError deserializeJson(TDocument& doc, TChar* input);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TChar* = char*, const char*, const FlashStringHelper*
template <typename TDocument, typename TChar>
JsonError deserializeJson(TDocument &doc, TChar *input) {
  using namespace Internals;
  return makeJsonDeserializer(&doc.buffer(), makeReader(input),
                              makeWriter(doc.buffer(), input), doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
//
// JsonError deserializeJson(TDocument& doc, TChar* input, size_t inputSize);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TChar* = char*, const char*, const FlashStringHelper*
template <typename TDocument, typename TChar>
JsonError deserializeJson(TDocument &doc, TChar *input, size_t inputSize) {
  using namespace Internals;
  return makeJsonDeserializer(&doc.buffer(), makeReader(input, inputSize),
                              makeWriter(doc.buffer(), input), doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
//
// JsonError deserializeJson(TDocument& doc, TStream input);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TStream = std::istream&, Stream&
template <typename TDocument, typename TStream>
JsonError deserializeJson(TDocument &doc, TStream &input) {
  using namespace Internals;
  return makeJsonDeserializer(&doc.buffer(), makeReader(input),
                              makeWriter(doc.buffer(), input), doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
}  // namespace ArduinoJson
