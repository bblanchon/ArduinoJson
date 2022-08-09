// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Deserialization/DeserializationError.hpp>
#include <ArduinoJson/Deserialization/Filter.hpp>
#include <ArduinoJson/Deserialization/NestingLimit.hpp>
#include <ArduinoJson/Deserialization/Reader.hpp>
#include <ArduinoJson/StringStorage/StringStorage.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <template <typename, typename> class TDeserializer, typename TReader,
          typename TWriter>
TDeserializer<TReader, TWriter> makeDeserializer(MemoryPool* pool,
                                                 TReader reader,
                                                 TWriter writer) {
  ARDUINOJSON_ASSERT(pool != 0);
  return TDeserializer<TReader, TWriter>(pool, reader, writer);
}

// deserialize(JsonDocument&, const std::string&, NestingLimit, Filter);
// deserialize(JsonDocument&, const String&, NestingLimit, Filter);
// deserialize(JsonDocument&, char*, NestingLimit, Filter);
// deserialize(JsonDocument&, const char*, NestingLimit, Filter);
// deserialize(JsonDocument&, const __FlashStringHelper*, NestingLimit, Filter);
template <template <typename, typename> class TDeserializer, typename TString,
          typename TFilter>
typename enable_if<!is_array<TString>::value, DeserializationError>::type
deserialize(JsonDocument& doc, const TString& input, NestingLimit nestingLimit,
            TFilter filter) {
  Reader<TString> reader(input);
  VariantData* data = VariantAttorney::getData(doc);
  MemoryPool* pool = VariantAttorney::getPool(doc);
  doc.clear();
  return makeDeserializer<TDeserializer>(pool, reader,
                                         makeStringStorage(input, pool))
      .parse(*data, filter, nestingLimit);
}
//
// deserialize(JsonDocument&, char*, size_t, NestingLimit, Filter);
// deserialize(JsonDocument&, const char*, size_t, NestingLimit, Filter);
// deserialize(JsonDocument&, const __FlashStringHelper*, size_t, NL, Filter);
template <template <typename, typename> class TDeserializer, typename TChar,
          typename TFilter>
DeserializationError deserialize(JsonDocument& doc, TChar* input,
                                 size_t inputSize, NestingLimit nestingLimit,
                                 TFilter filter) {
  BoundedReader<TChar*> reader(input, inputSize);
  VariantData* data = VariantAttorney::getData(doc);
  MemoryPool* pool = VariantAttorney::getPool(doc);
  doc.clear();
  return makeDeserializer<TDeserializer>(pool, reader,
                                         makeStringStorage(input, pool))
      .parse(*data, filter, nestingLimit);
}
//
// deserialize(JsonDocument&, std::istream&, NestingLimit, Filter);
// deserialize(JsonDocument&, Stream&, NestingLimit, Filter);
template <template <typename, typename> class TDeserializer, typename TStream,
          typename TFilter>
DeserializationError deserialize(JsonDocument& doc, TStream& input,
                                 NestingLimit nestingLimit, TFilter filter) {
  Reader<TStream> reader(input);
  VariantData* data = VariantAttorney::getData(doc);
  MemoryPool* pool = VariantAttorney::getPool(doc);
  doc.clear();
  return makeDeserializer<TDeserializer>(pool, reader,
                                         makeStringStorage(input, pool))
      .parse(*data, filter, nestingLimit);
}

}  // namespace ARDUINOJSON_NAMESPACE
