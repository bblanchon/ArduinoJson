// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Serialization/Writers/DummyWriter.hpp>
#include <ArduinoJson/Variant/VariantFunctions.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <template <typename> class TSerializer>
size_t measure(ArduinoJson::JsonVariantConst source) {
  DummyWriter dp;
  TSerializer<DummyWriter> serializer(dp);
  return variantAccept(VariantAttorney::getData(source), serializer);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
