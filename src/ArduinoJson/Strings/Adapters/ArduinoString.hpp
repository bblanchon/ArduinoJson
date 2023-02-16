// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <Arduino.h>

#include <ArduinoJson/Strings/Adapters/RamString.hpp>
#include <ArduinoJson/Strings/StringAdapter.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename T>
struct StringAdapter<
    T, typename enable_if<is_same<T, ::String>::value ||
                          is_same<T, ::StringSumHelper>::value>::type> {
  typedef SizedRamString AdaptedString;

  static AdaptedString adapt(const ::String& s) {
    return AdaptedString(s.c_str(), s.length());
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
