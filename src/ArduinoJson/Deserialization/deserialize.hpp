// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
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

// A meta-function that returns true if T is a valid destination type for
// deserialize()
template <class T, class = void>
struct is_deserialize_destination : false_type {};

template <class T>
struct is_deserialize_destination<
    T, typename enable_if<is_same<decltype(VariantAttorney::getResourceManager(
                                      detail::declval<T&>())),
                                  ResourceManager*>::value>::type> : true_type {
};

template <typename TDestination>
inline void shrinkJsonDocument(TDestination&) {
  // no-op by default
}

#if ARDUINOJSON_AUTO_SHRINK
inline void shrinkJsonDocument(JsonDocument& doc) {
  doc.shrinkToFit();
}
#endif

template <template <typename> class TDeserializer, typename TDestination,
          typename TReader, typename TOptions>
DeserializationError doDeserialize(TDestination&& dst, TReader reader,
                                   TOptions options) {
  auto data = VariantAttorney::getOrCreateData(dst);
  if (!data)
    return DeserializationError::NoMemory;
  auto resources = VariantAttorney::getResourceManager(dst);
  dst.clear();
  auto err = TDeserializer<TReader>(resources, reader)
                 .parse(*data, options.filter, options.nestingLimit);
  shrinkJsonDocument(dst);
  return err;
}

template <template <typename> class TDeserializer, typename TDestination,
          typename TStream, typename... Args,
          typename = typename enable_if<  // issue #1897
              !is_integral<typename first_or_void<Args...>::type>::value>::type>
DeserializationError deserialize(TDestination&& dst, TStream&& input,
                                 Args... args) {
  return doDeserialize<TDeserializer>(
      dst, makeReader(detail::forward<TStream>(input)),
      makeDeserializationOptions(args...));
}

template <template <typename> class TDeserializer, typename TDestination,
          typename TChar, typename Size, typename... Args,
          typename = typename enable_if<is_integral<Size>::value>::type>
DeserializationError deserialize(TDestination&& dst, TChar* input,
                                 Size inputSize, Args... args) {
  return doDeserialize<TDeserializer>(dst, makeReader(input, size_t(inputSize)),
                                      makeDeserializationOptions(args...));
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
