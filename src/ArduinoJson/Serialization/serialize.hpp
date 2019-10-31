// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
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
size_t serialize(const TSource &source, char *buffer, size_t bufferSize) {
  StaticStringWriter writer(buffer, bufferSize);
  return doSerialize<TSerializer>(source, writer);
}

template <template <typename> class TSerializer, typename TSource, size_t N>
size_t serialize(const TSource &source, char (&buffer)[N]) {
  StaticStringWriter writer(buffer, N);
  return doSerialize<TSerializer>(source, writer);
}

}  // namespace ARDUINOJSON_NAMESPACE
