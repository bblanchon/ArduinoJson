// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Operators/VariantComparisons.hpp>
#include <ArduinoJson/Operators/VariantOr.hpp>
#include <ArduinoJson/Operators/VariantShortcuts.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TImpl>
class VariantOperators : public VariantComparisons<TImpl>,
                         public VariantOr<TImpl>,
                         public VariantShortcuts<TImpl> {};
}  // namespace ARDUINOJSON_NAMESPACE
