// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Strings/Adapters/RamString.hpp>

#include <string>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TCharTraits, typename TAllocator>
struct StringAdapter<std::basic_string<char, TCharTraits, TAllocator>, void> {
  typedef SizedRamString AdaptedString;

  static AdaptedString adapt(
      const std::basic_string<char, TCharTraits, TAllocator>& s) {
    return AdaptedString(s.c_str(), s.size());
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
