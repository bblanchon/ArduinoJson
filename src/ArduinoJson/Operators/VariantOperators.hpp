// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Operators/VariantCasts.hpp>
#include <ArduinoJson/Operators/VariantComparisons.hpp>
#include <ArduinoJson/Operators/VariantOr.hpp>
#include <ArduinoJson/Operators/VariantShortcuts.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TImpl>
class VariantOperators : public VariantCasts<TImpl>,
                         public VariantComparisons<TImpl>,
                         public VariantOr<TImpl>,
                         public VariantShortcuts<TImpl> {};
}  // namespace ARDUINOJSON_NAMESPACE
