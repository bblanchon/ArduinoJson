// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Serialization/Writer.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <template <typename> class TSerializer, typename TWriter>
size_t doSerialize(ArduinoJson::JsonVariantConst source, TWriter writer) {
  TSerializer<TWriter> serializer(writer,
                                  VariantAttorney::getResourceManager(source));
  return VariantData::accept(VariantAttorney::getData(source), serializer);
}

template <template <typename> class TSerializer, typename TDestination>
size_t serialize(ArduinoJson::JsonVariantConst source,
                 TDestination& destination) {
  Writer<TDestination> writer(destination);
  return doSerialize<TSerializer>(source, writer);
}

template <template <typename> class TSerializer>
enable_if_t<!TSerializer<StaticStringWriter>::producesText, size_t> serialize(
    ArduinoJson::JsonVariantConst source, void* buffer, size_t bufferSize) {
  StaticStringWriter writer(reinterpret_cast<char*>(buffer), bufferSize);
  return doSerialize<TSerializer>(source, writer);
}

template <template <typename> class TSerializer>
enable_if_t<TSerializer<StaticStringWriter>::producesText, size_t> serialize(
    ArduinoJson::JsonVariantConst source, void* buffer, size_t bufferSize) {
  StaticStringWriter writer(reinterpret_cast<char*>(buffer), bufferSize);
  size_t n = doSerialize<TSerializer>(source, writer);
  // add null-terminator for text output (not counted in the size)
  if (n < bufferSize)
    reinterpret_cast<char*>(buffer)[n] = 0;
  return n;
}

template <template <typename> class TSerializer, typename TChar, size_t N>
enable_if_t<IsChar<TChar>::value, size_t> serialize(
    ArduinoJson::JsonVariantConst source, TChar (&buffer)[N]) {
  return serialize<TSerializer>(source, buffer, N);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
