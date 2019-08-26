// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Serialization/DynamicStringWriter.hpp>
#include <ArduinoJson/Serialization/StaticStringWriter.hpp>

#if ARDUINOJSON_ENABLE_STD_STREAM
#include <ArduinoJson/Serialization/StreamWriter.hpp>
#endif

namespace ARDUINOJSON_NAMESPACE {

template <template <typename> class TSerializer, typename TSource,
          typename TDestination>
size_t doSerialize(const TSource &source, TDestination &destination) {
  TSerializer<TDestination> serializer(destination);
  source.accept(serializer);
  return serializer.bytesWritten();
}

#if ARDUINOJSON_ENABLE_STD_STREAM
template <template <typename> class TSerializer, typename TSource>
size_t serialize(const TSource &source, std::ostream &destination) {
  StreamWriter writer(destination);
  return doSerialize<TSerializer>(source, writer);
}
#endif

#if ARDUINOJSON_ENABLE_ARDUINO_PRINT
template <template <typename> class TSerializer, typename TSource>
size_t serialize(const TSource &source, Print &destination) {
  return doSerialize<TSerializer>(source, destination);
}
#endif

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

template <template <typename> class TSerializer, typename TSource,
          typename TString>
typename enable_if<IsWriteableString<TString>::value, size_t>::type serialize(
    const TSource &source, TString &str) {
  DynamicStringWriter<TString> writer(str);
  return doSerialize<TSerializer>(source, writer);
}

}  // namespace ARDUINOJSON_NAMESPACE
