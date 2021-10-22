// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/type_traits.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename T>
class SafeBoolIdom {
 protected:
  typedef void (T::*bool_type)() const;
  void safeBoolHelper() const {}

  static bool_type safe_true() {
    return &SafeBoolIdom::safeBoolHelper;
  }

  static bool_type safe_false() {
    return 0;
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
