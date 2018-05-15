// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "MsgPack/MsgPackDeserializer.hpp"
#include "Reading/Reader.hpp"
#include "Writing/Writer.hpp"

namespace ArduinoJson {
// DeserializationError deserializeMsgPack(TDocument& doc, TString input);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TString = const std::string&, const String&
template <typename TDocument, typename TString>
typename Internals::EnableIf<!Internals::IsArray<TString>::value,
                             DeserializationError>::type
deserializeMsgPack(TDocument &doc, const TString &input) {
  using namespace Internals;
  return makeMsgPackDeserializer(&doc.buffer(), makeReader(input),
                                 makeWriter(doc.buffer(), input),
                                 doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
//
// DeserializationError deserializeMsgPack(TDocument& doc, TChar* input);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TChar* = char*, const char*, const FlashStringHelper*
template <typename TDocument, typename TChar>
DeserializationError deserializeMsgPack(TDocument &doc, TChar *input) {
  using namespace Internals;
  return makeMsgPackDeserializer(&doc.buffer(), makeReader(input),
                                 makeWriter(doc.buffer(), input),
                                 doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
//
// DeserializationError deserializeMsgPack(TDocument& doc, TChar* input, size_t
// inputSize);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TChar* = char*, const char*, const FlashStringHelper*
template <typename TDocument, typename TChar>
DeserializationError deserializeMsgPack(TDocument &doc, TChar *input,
                                        size_t inputSize) {
  using namespace Internals;
  return makeMsgPackDeserializer(&doc.buffer(), makeReader(input, inputSize),
                                 makeWriter(doc.buffer(), input),
                                 doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
//
// DeserializationError deserializeMsgPack(TDocument& doc, TStream input);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TStream = std::istream&, Stream&
template <typename TDocument, typename TStream>
DeserializationError deserializeMsgPack(TDocument &doc, TStream &input) {
  using namespace Internals;
  return makeMsgPackDeserializer(&doc.buffer(), makeReader(input),
                                 makeWriter(doc.buffer(), input),
                                 doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
}  // namespace ArduinoJson
