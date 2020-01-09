// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Deserialization/DeserializationError.hpp>
#include <ArduinoJson/Deserialization/NestingLimit.hpp>
#include <ArduinoJson/Deserialization/Reader.hpp>
#include <ArduinoJson/StringStorage/StringStorage.hpp>

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
// deserialize(JsonDocument&, char*);
// deserialize(JsonDocument&, const char*);
// deserialize(JsonDocument&, const __FlashStringHelper*);
template <template <typename, typename> class TDeserializer, typename TString>
typename enable_if<!is_array<TString>::value, DeserializationError>::type
deserialize(JsonDocument &doc, const TString &input,
            NestingLimit nestingLimit) {
  Reader<TString> reader(input);
  doc.clear();
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), reader,
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
  BoundedReader<TChar *> reader(input, inputSize);
  doc.clear();
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), reader,
             makeStringStorage(doc.memoryPool(), input), nestingLimit.value)
      .parse(doc.data());
}
//
// deserialize(JsonDocument&, std::istream&);
// deserialize(JsonDocument&, Stream&);
template <template <typename, typename> class TDeserializer, typename TStream>
DeserializationError deserialize(JsonDocument &doc, TStream &input,
                                 NestingLimit nestingLimit) {
  Reader<TStream> reader(input);
  doc.clear();
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), reader,
             makeStringStorage(doc.memoryPool(), input), nestingLimit.value)
      .parse(doc.data());
}
}  // namespace ARDUINOJSON_NAMESPACE
