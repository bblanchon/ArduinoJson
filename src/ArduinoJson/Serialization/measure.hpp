// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Serialization/Writers/DummyWriter.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <template <typename> class TSerializer>
size_t measure(ArduinoJson::JsonVariantConst source) {
  DummyWriter dp;
  TSerializer<DummyWriter> serializer(
      dp, VariantAttorney::getResourceManager(source));
  return VariantData::accept(VariantAttorney::getData(source), serializer);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
