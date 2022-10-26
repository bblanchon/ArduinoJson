// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Strings/Adapters/RamString.hpp>

#include <string>

namespace ARDUINOJSON_NAMESPACE {

template <typename TCharTraits, typename TAllocator>
struct StringAdapter<std::basic_string<char, TCharTraits, TAllocator>, void> {
  typedef SizedRamString AdaptedString;

  static AdaptedString adapt(
      const std::basic_string<char, TCharTraits, TAllocator>& s) {
    return AdaptedString(s.c_str(), s.size());
  }
};

template <typename TCharTraits, typename TAllocator>
struct IsString<std::basic_string<char, TCharTraits, TAllocator> > : true_type {
};

}  // namespace ARDUINOJSON_NAMESPACE
