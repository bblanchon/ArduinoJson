// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Serialization/DummyWriter.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <template <typename> class TSerializer, typename TSource>
size_t measure(const TSource &source) {
  DummyWriter dp;
  TSerializer<DummyWriter> serializer(dp);
  source.accept(serializer);
  return serializer.bytesWritten();
}

}  // namespace ARDUINOJSON_NAMESPACE
