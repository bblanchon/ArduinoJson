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

namespace ArduinoJson {
namespace Internals {

template <template <typename, typename> class TDeserializer,
          typename TMemoryPool, typename TReader, typename TWriter>
TDeserializer<TReader, TWriter> makeDeserializer(TMemoryPool &memoryPool,
                                                 TReader reader, TWriter writer,
                                                 uint8_t nestingLimit) {
  return TDeserializer<TReader, TWriter>(memoryPool, reader, writer,
                                         nestingLimit);
}

// DeserializationError deserialize(TDocument& doc, TString input);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TString = const std::string&, const String&
template <template <typename, typename> class TDeserializer, typename TDocument,
          typename TString>
typename Internals::enable_if<!Internals::is_array<TString>::value,
                              DeserializationError>::type
deserialize(TDocument &doc, const TString &input) {
  using namespace Internals;
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), makeReader(input),
             makeStringStorage(doc.memoryPool(), input), doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
//
// DeserializationError deserialize(TDocument& doc, TChar* input);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TChar* = char*, const char*, const FlashStringHelper*
template <template <typename, typename> class TDeserializer, typename TDocument,
          typename TChar>
DeserializationError deserialize(TDocument &doc, TChar *input) {
  using namespace Internals;
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), makeReader(input),
             makeStringStorage(doc.memoryPool(), input), doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
//
// DeserializationError deserialize(TDocument& doc, TChar* input, size_t
// inputSize);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TChar* = char*, const char*, const FlashStringHelper*
template <template <typename, typename> class TDeserializer, typename TDocument,
          typename TChar>
DeserializationError deserialize(TDocument &doc, TChar *input,
                                 size_t inputSize) {
  using namespace Internals;
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), makeReader(input, inputSize),
             makeStringStorage(doc.memoryPool(), input), doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
//
// DeserializationError deserialize(TDocument& doc, TStream input);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TStream = std::istream&, Stream&
template <template <typename, typename> class TDeserializer, typename TDocument,
          typename TStream>
DeserializationError deserialize(TDocument &doc, TStream &input) {
  using namespace Internals;
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), makeReader(input),
             makeStringStorage(doc.memoryPool(), input), doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
}  // namespace Internals
}  // namespace ArduinoJson
