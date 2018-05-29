// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "./DynamicStringBuilder.hpp"
#include "./StaticStringBuilder.hpp"

#if ARDUINOJSON_ENABLE_STD_STREAM
#include "./StreamPrintAdapter.hpp"
#endif

namespace ArduinoJson {
namespace Internals {

template <template <typename> class TSerializer, typename TSource,
          typename TPrint>
typename enable_if<!StringTraits<TPrint>::has_append, size_t>::type serialize(
    const TSource &source, TPrint &destination) {
  TSerializer<TPrint> serializer(destination);
  source.visit(serializer);
  return serializer.bytesWritten();
}

#if ARDUINOJSON_ENABLE_STD_STREAM
template <template <typename> class TSerializer, typename TSource>
size_t serialize(const TSource &source, std::ostream &os) {
  StreamPrintAdapter adapter(os);
  return serialize<TSerializer>(source, adapter);
}
#endif

template <template <typename> class TSerializer, typename TSource>
size_t serialize(const TSource &source, char *buffer, size_t bufferSize) {
  StaticStringBuilder sb(buffer, bufferSize);
  return serialize<TSerializer>(source, sb);
}

template <template <typename> class TSerializer, typename TSource, size_t N>
size_t serialize(const TSource &source, char (&buffer)[N]) {
  StaticStringBuilder sb(buffer, N);
  return serialize<TSerializer>(source, sb);
}

template <template <typename> class TSerializer, typename TSource,
          typename TString>
typename enable_if<StringTraits<TString>::has_append, size_t>::type serialize(
    const TSource &source, TString &str) {
  DynamicStringBuilder<TString> sb(str);
  return serialize<TSerializer>(source, sb);
}

}  // namespace Internals
}  // namespace ArduinoJson
