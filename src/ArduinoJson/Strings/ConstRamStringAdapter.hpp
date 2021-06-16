// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <stddef.h>  // size_t
#include <string.h>  // strcmp

#include <ArduinoJson/Polyfills/safe_strcmp.hpp>
#include <ArduinoJson/Strings/IsString.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>

namespace ARDUINOJSON_NAMESPACE {

class ConstRamStringAdapter {
 public:
  ConstRamStringAdapter(const char* str = 0) : _str(str) {}

  int compare(const char* other) const {
    return safe_strcmp(_str, other);
  }

  bool equals(const char* expected) const {
    return compare(expected) == 0;
  }

  bool isNull() const {
    return !_str;
  }

  size_t size() const {
    if (!_str)
      return 0;
    return strlen(_str);
  }

  const char* data() const {
    return _str;
  }

  typedef storage_policies::store_by_address storage_policy;

 protected:
  const char* _str;
};

template <>
struct IsString<const char*> : true_type {};

template <int N>
struct IsString<const char[N]> : true_type {};

inline ConstRamStringAdapter adaptString(const char* str) {
  return ConstRamStringAdapter(str);
}

}  // namespace ARDUINOJSON_NAMESPACE
