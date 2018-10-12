// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonVariantCasts.hpp"
#include "JsonVariantComparisons.hpp"
#include "JsonVariantOr.hpp"
#include "JsonVariantSubscripts.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TImpl>
class JsonVariantBase : public JsonVariantCasts<TImpl>,
                        public JsonVariantComparisons<TImpl>,
                        public JsonVariantOr<TImpl>,
                        public JsonVariantSubscripts<TImpl> {};
}  // namespace ARDUINOJSON_NAMESPACE
