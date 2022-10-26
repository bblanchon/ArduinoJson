// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Strings/Adapters/RamString.hpp>

#include <string_view>

namespace ARDUINOJSON_NAMESPACE {

template <>
struct StringAdapter<std::string_view, void> {
  typedef SizedRamString AdaptedString;

  static AdaptedString adapt(const std::string_view& s) {
    return AdaptedString(s.data(), s.size());
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
