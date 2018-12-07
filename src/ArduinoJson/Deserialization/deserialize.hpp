// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../StringStorage/StringStorage.hpp"
#include "./ArduinoStreamReader.hpp"
#include "./CharPointerReader.hpp"
#include "./DeserializationError.hpp"
#include "./FlashStringReader.hpp"
#include "./IteratorReader.hpp"
#include "./StdStreamReader.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <template <typename, typename> class TDeserializer, typename TReader,
          typename TWriter>
TDeserializer<TReader, TWriter> makeDeserializer(MemoryPool &pool,
                                                 TReader reader, TWriter writer,
                                                 uint8_t nestingLimit) {
  return TDeserializer<TReader, TWriter>(pool, reader, writer, nestingLimit);
}

// DeserializationError deserialize(JsonDocument& doc, TString input);
// TString = const std::string&, const String&
template <template <typename, typename> class TDeserializer, typename TString>
typename enable_if<!is_array<TString>::value, DeserializationError>::type
deserialize(JsonDocument &doc, const TString &input) {
  doc.clear();
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), makeReader(input),
             makeStringStorage(doc.memoryPool(), input), doc.nestingLimit)
      .parse(doc.data());
}
//
// DeserializationError deserialize(JsonDocument& doc, TChar* input);
// TChar* = char*, const char*, const __FlashStringHelper*
template <template <typename, typename> class TDeserializer, typename TChar>
DeserializationError deserialize(JsonDocument &doc, TChar *input) {
  doc.clear();
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), makeReader(input),
             makeStringStorage(doc.memoryPool(), input), doc.nestingLimit)
      .parse(doc.data());
}
//
// DeserializationError deserialize(JsonDocument& doc, TChar* input, size_t
// inputSize);
// TChar* = char*, const char*, const __FlashStringHelper*
template <template <typename, typename> class TDeserializer, typename TChar>
DeserializationError deserialize(JsonDocument &doc, TChar *input,
                                 size_t inputSize) {
  doc.clear();
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), makeReader(input, inputSize),
             makeStringStorage(doc.memoryPool(), input), doc.nestingLimit)
      .parse(doc.data());
}
//
// DeserializationError deserialize(JsonDocument& doc, TStream input);
// TStream = std::istream&, Stream&
template <template <typename, typename> class TDeserializer, typename TStream>
DeserializationError deserialize(JsonDocument &doc, TStream &input) {
  doc.clear();
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), makeReader(input),
             makeStringStorage(doc.memoryPool(), input), doc.nestingLimit)
      .parse(doc.data());
}
}  // namespace ARDUINOJSON_NAMESPACE
