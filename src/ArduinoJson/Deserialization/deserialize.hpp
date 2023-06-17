// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Deserialization/DeserializationError.hpp>
#include <ArduinoJson/Deserialization/DeserializationOptions.hpp>
#include <ArduinoJson/Deserialization/Reader.hpp>
#include <ArduinoJson/Polyfills/utility.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

// A meta-function that returns the first type of the parameter pack
// or void if empty
template <typename...>
struct first_or_void {
  using type = void;
};
template <typename T, typename... Rest>
struct first_or_void<T, Rest...> {
  using type = T;
};

template <template <typename> class TDeserializer, typename TReader>
TDeserializer<TReader> makeDeserializer(ResourceManager* resources,
                                        TReader reader) {
  ARDUINOJSON_ASSERT(resources != 0);
  return TDeserializer<TReader>(resources, reader);
}

template <template <typename> class TDeserializer, typename TStream,
          typename... Args,
          typename = typename enable_if<  // issue #1897
              !is_integral<typename first_or_void<Args...>::type>::value>::type>
DeserializationError deserialize(JsonDocument& doc, TStream&& input,
                                 Args... args) {
  auto reader = makeReader(detail::forward<TStream>(input));
  auto data = VariantAttorney::getData(doc);
  auto resources = VariantAttorney::getResourceManager(doc);
  auto options = makeDeserializationOptions(args...);
  doc.clear();
  return makeDeserializer<TDeserializer>(resources, reader)
      .parse(*data, options.filter, options.nestingLimit);
}

template <template <typename> class TDeserializer, typename TChar,
          typename Size, typename... Args,
          typename = typename enable_if<is_integral<Size>::value>::type>
DeserializationError deserialize(JsonDocument& doc, TChar* input,
                                 Size inputSize, Args... args) {
  auto reader = makeReader(input, size_t(inputSize));
  auto data = VariantAttorney::getData(doc);
  auto resources = VariantAttorney::getResourceManager(doc);
  auto options = makeDeserializationOptions(args...);
  doc.clear();
  return makeDeserializer<TDeserializer>(resources, reader)
      .parse(*data, options.filter, options.nestingLimit);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
