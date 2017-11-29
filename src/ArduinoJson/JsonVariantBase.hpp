// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2017
// MIT License

#pragma once

#include "JsonVariantCasts.hpp"
#include "JsonVariantComparisons.hpp"
#include "JsonVariantOr.hpp"
#include "JsonVariantSubscripts.hpp"
#include "Serialization/JsonPrintable.hpp"

namespace ArduinoJson {

template <typename TImpl>
class JsonVariantBase : public Internals::JsonPrintable<TImpl>,
                        public JsonVariantCasts<TImpl>,
                        public JsonVariantComparisons<TImpl>,
                        public JsonVariantOr<TImpl>,
                        public JsonVariantSubscripts<TImpl>,
                        public TypeTraits::JsonVariantTag {};
}
