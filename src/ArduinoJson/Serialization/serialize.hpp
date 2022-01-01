// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Serialization/Writer.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <template <typename> class TSerializer, typename TSource,
          typename TWriter>
size_t doSerialize(const TSource &source, TWriter writer) {
  TSerializer<TWriter> serializer(writer);
  return source.accept(serializer);
}

template <template <typename> class TSerializer, typename TSource,
          typename TDestination>
size_t serialize(const TSource &source, TDestination &destination) {
  Writer<TDestination> writer(destination);
  return doSerialize<TSerializer>(source, writer);
}

template <template <typename> class TSerializer, typename TSource>
typename enable_if<!TSerializer<StaticStringWriter>::producesText, size_t>::type
serialize(const TSource &source, void *buffer, size_t bufferSize) {
  StaticStringWriter writer(reinterpret_cast<char *>(buffer), bufferSize);
  return doSerialize<TSerializer>(source, writer);
}

template <template <typename> class TSerializer, typename TSource>
typename enable_if<TSerializer<StaticStringWriter>::producesText, size_t>::type
serialize(const TSource &source, void *buffer, size_t bufferSize) {
  StaticStringWriter writer(reinterpret_cast<char *>(buffer), bufferSize);
  size_t n = doSerialize<TSerializer>(source, writer);
  // add null-terminator for text output (not counted in the size)
  if (n < bufferSize)
    reinterpret_cast<char *>(buffer)[n] = 0;
  return n;
}

template <template <typename> class TSerializer, typename TSource,
          typename TChar, size_t N>
#if defined _MSC_VER && _MSC_VER < 1900
typename enable_if<sizeof(remove_reference<TChar>::type) == 1, size_t>::type
#else
typename enable_if<sizeof(TChar) == 1, size_t>::type
#endif
serialize(const TSource &source, TChar (&buffer)[N]) {
  return serialize<TSerializer>(source, buffer, N);
}

}  // namespace ARDUINOJSON_NAMESPACE
