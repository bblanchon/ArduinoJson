// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Strings/Adapters/RamString.hpp>

#include <string>

namespace ARDUINOJSON_NAMESPACE {

template <typename TCharTraits, typename TAllocator>
inline SizedRamString adaptString(
    const std::basic_string<char, TCharTraits, TAllocator>& s) {
  return SizedRamString(s.c_str(), s.size());
}

template <typename TCharTraits, typename TAllocator>
struct IsString<std::basic_string<char, TCharTraits, TAllocator> > : true_type {
};

}  // namespace ARDUINOJSON_NAMESPACE
