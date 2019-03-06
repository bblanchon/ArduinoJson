// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "../Polyfills/type_traits.hpp"
#include "convertNumber.hpp"
#include "parseNumber.hpp"

namespace ARDUINOJSON_NAMESPACE {
template <typename T>
T parseInteger(const char *s) {
  // try to reuse the same parameters as JsonDeserializer
  typedef typename choose_largest<UInt, typename make_unsigned<T>::type>::type
      TUInt;
  return parseNumber<Float, TUInt>(s).template as<T>();
}
}  // namespace ARDUINOJSON_NAMESPACE
