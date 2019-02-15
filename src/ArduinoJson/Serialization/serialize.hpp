// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "./DynamicStringWriter.hpp"
#include "./StaticStringWriter.hpp"

#if ARDUINOJSON_ENABLE_STD_STREAM
#include "./StreamWriter.hpp"
#endif

namespace ARDUINOJSON_NAMESPACE {

template <template <typename> class TSerializer, typename TSource,
          typename TPrint>
typename enable_if<!IsWriteableString<TPrint>::value, size_t>::type serialize(
    const TSource &source, TPrint &destination) {
  TSerializer<TPrint> serializer(destination);
  source.accept(serializer);
  return serializer.bytesWritten();
}

#if ARDUINOJSON_ENABLE_STD_STREAM
template <template <typename> class TSerializer, typename TSource>
size_t serialize(const TSource &source, std::ostream &os) {
  StreamWriter writer(os);
  return serialize<TSerializer>(source, writer);
}
#endif

template <template <typename> class TSerializer, typename TSource>
size_t serialize(const TSource &source, char *buffer, size_t bufferSize) {
  StaticStringWriter writer(buffer, bufferSize);
  return serialize<TSerializer>(source, writer);
}

template <template <typename> class TSerializer, typename TSource, size_t N>
size_t serialize(const TSource &source, char (&buffer)[N]) {
  StaticStringWriter writer(buffer, N);
  return serialize<TSerializer>(source, writer);
}

template <template <typename> class TSerializer, typename TSource,
          typename TString>
typename enable_if<IsWriteableString<TString>::value, size_t>::type serialize(
    const TSource &source, TString &str) {
  DynamicStringWriter<TString> writer(str);
  return serialize<TSerializer>(source, writer);
}

}  // namespace ARDUINOJSON_NAMESPACE
