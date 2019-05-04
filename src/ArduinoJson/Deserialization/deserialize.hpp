// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "../StringStorage/StringStorage.hpp"
#include "ArduinoStreamReader.hpp"
#include "CharPointerReader.hpp"
#include "DeserializationError.hpp"
#include "FlashStringReader.hpp"
#include "IteratorReader.hpp"
#include "NestingLimit.hpp"
#include "StdStreamReader.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <template <typename, typename> class TDeserializer, typename TReader,
          typename TWriter>
TDeserializer<TReader, TWriter> makeDeserializer(MemoryPool &pool,
                                                 TReader reader, TWriter writer,
                                                 uint8_t nestingLimit) {
  return TDeserializer<TReader, TWriter>(pool, reader, writer, nestingLimit);
}

// deserialize(JsonDocument&, const std::string&);
// deserialize(JsonDocument&, const String&);
template <template <typename, typename> class TDeserializer, typename TString>
typename enable_if<!is_array<TString>::value, DeserializationError>::type
deserialize(JsonDocument &doc, const TString &input,
            NestingLimit nestingLimit) {
  doc.clear();
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), makeReader(input),
             makeStringStorage(doc.memoryPool(), input), nestingLimit.value)
      .parse(doc.data());
}
//
// deserialize(JsonDocument&, char*);
// deserialize(JsonDocument&, const char*);
// deserialize(JsonDocument&, const __FlashStringHelper*);
template <template <typename, typename> class TDeserializer, typename TChar>
#if ARDUINOJSON_ENABLE_ARDUINO_STREAM
typename enable_if<!is_base_of<Stream, TChar>::value,
                   DeserializationError>::type
#else
DeserializationError
#endif
deserialize(JsonDocument &doc, TChar *input, NestingLimit nestingLimit) {
  doc.clear();
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), makeReader(input),
             makeStringStorage(doc.memoryPool(), input), nestingLimit.value)
      .parse(doc.data());
}
//
// deserialize(JsonDocument&, char*, size_t);
// deserialize(JsonDocument&, const char*, size_t);
// deserialize(JsonDocument&, const __FlashStringHelper*, size_t);
template <template <typename, typename> class TDeserializer, typename TChar>
DeserializationError deserialize(JsonDocument &doc, TChar *input,
                                 size_t inputSize, NestingLimit nestingLimit) {
  doc.clear();
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), makeReader(input, inputSize),
             makeStringStorage(doc.memoryPool(), input), nestingLimit.value)
      .parse(doc.data());
}
//
// deserialize(JsonDocument&, std::istream&);
// deserialize(JsonDocument&, Stream&);
template <template <typename, typename> class TDeserializer, typename TStream>
DeserializationError deserialize(JsonDocument &doc, TStream &input,
                                 NestingLimit nestingLimit) {
  doc.clear();
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), makeReader(input),
             makeStringStorage(doc.memoryPool(), input), nestingLimit.value)
      .parse(doc.data());
}
#if ARDUINOJSON_ENABLE_ARDUINO_STREAM
//
// deserialize(JsonDocument&, Stream*);
template <template <typename, typename> class TDeserializer, typename TStream>
typename enable_if<is_base_of<Stream, TStream>::value,
                   DeserializationError>::type
deserialize(JsonDocument &doc, TStream *input, NestingLimit nestingLimit) {
  doc.clear();
  if (!input) return DeserializationError::NoMemory;
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), makeReader(*input),
             makeStringStorage(doc.memoryPool(), *input), nestingLimit.value)
      .parse(doc.data());
}
#endif
}  // namespace ARDUINOJSON_NAMESPACE
