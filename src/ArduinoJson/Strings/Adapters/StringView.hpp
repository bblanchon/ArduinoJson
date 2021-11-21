// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Strings/Adapters/RamString.hpp>

#include <string_view>

namespace ARDUINOJSON_NAMESPACE {

inline SizedRamString adaptString(const std::string_view& s) {
  return SizedRamString(s.data(), s.size());
}

template <>
struct IsString<std::string_view> : true_type {};

}  // namespace ARDUINOJSON_NAMESPACE
