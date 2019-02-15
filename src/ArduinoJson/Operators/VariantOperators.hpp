// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "VariantCasts.hpp"
#include "VariantComparisons.hpp"
#include "VariantOr.hpp"
#include "VariantShortcuts.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TImpl>
class VariantOperators : public VariantCasts<TImpl>,
                         public VariantComparisons<TImpl>,
                         public VariantOr<TImpl>,
                         public VariantShortcuts<TImpl> {};
}  // namespace ARDUINOJSON_NAMESPACE
