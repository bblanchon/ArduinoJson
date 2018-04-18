// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "MsgPack/MsgPackDeserializer.hpp"

namespace ArduinoJson {
// MsgPackError deserializeMsgPack(TDocument& doc, TString json);
// TDocument = DynamicJsonArray | StaticJsonArray
// TString = const std::string&, const String&
template <typename TDocument, typename TString>
typename Internals::EnableIf<!Internals::IsArray<TString>::value,
                             MsgPackError>::type
deserializeMsgPack(TDocument &doc, const TString &json) {
  return Internals::makeMsgPackDeserializer(&doc.buffer(), json,
                                            doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
//
// MsgPackError deserializeMsgPack(TDocument& doc, TString json);
// TDocument = DynamicJsonArray | StaticJsonArray
// TString = const char*, const char[N], const FlashStringHelper*
template <typename TDocument, typename TString>
MsgPackError deserializeMsgPack(TDocument &doc, TString *json) {
  return Internals::makeMsgPackDeserializer(&doc.buffer(), json,
                                            doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
//
// MsgPackError deserializeMsgPack(TDocument& doc, TString json);
// TDocument = DynamicJsonArray | StaticJsonArray
// TString = std::istream&, Stream&
template <typename TDocument, typename TString>
MsgPackError deserializeMsgPack(TDocument &doc, TString &json) {
  return Internals::makeMsgPackDeserializer(&doc.buffer(), json,
                                            doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
}  // namespace ArduinoJson
