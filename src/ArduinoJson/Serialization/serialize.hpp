// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Serialization/Writer.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <template <typename> class TSerializer, typename TSource,
          typename TWriter>
size_t doSerialize(const TSource &source, TWriter writer) {
  TSerializer<TWriter> serializer(writer);
  source.accept(serializer);
  return serializer.bytesWritten();
}

template <template <typename> class TSerializer, typename TSource,
          typename TDestination>
size_t serialize(const TSource &source, TDestination &destination) {
  Writer<TDestination> writer(destination);
  return doSerialize<TSerializer>(source, writer);
}

template <template <typename> class TSerializer, typename TSource>
size_t serialize(const TSource &source, void *buffer, size_t bufferSize) {
  StaticStringWriter writer(reinterpret_cast<char *>(buffer), bufferSize);
  return doSerialize<TSerializer>(source, writer);
}

template <template <typename> class TSerializer, typename TSource,
          typename TChar, size_t N>
#if defined _MSC_VER && _MSC_VER < 1900
typename enable_if<sizeof(remove_reference<TChar>::type) == 1, size_t>::type
#else
typename enable_if<sizeof(TChar) == 1, size_t>::type
#endif
serialize(const TSource &source, TChar (&buffer)[N]) {
  StaticStringWriter writer(reinterpret_cast<char *>(buffer), N);
  return doSerialize<TSerializer>(source, writer);
}

}  // namespace ARDUINOJSON_NAMESPACE
