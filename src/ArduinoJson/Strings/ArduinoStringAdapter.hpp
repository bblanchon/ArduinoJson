// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <Arduino.h>

#include <ArduinoJson/Polyfills/safe_strcmp.hpp>
#include <ArduinoJson/Strings/IsString.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>

namespace ARDUINOJSON_NAMESPACE {

class ArduinoStringAdapter {
 public:
  ArduinoStringAdapter(const ::String& str) : _str(&str) {}

  void copyTo(char* p, size_t n) const {
    memcpy(p, _str->c_str(), n);
  }

  bool isNull() const {
    // Arduino's String::c_str() can return NULL
    return !_str->c_str();
  }

  int compare(const char* other) const {
    // Arduino's String::c_str() can return NULL
    const char* me = _str->c_str();
    return safe_strcmp(me, other);
  }

  bool equals(const char* expected) const {
    return compare(expected) == 0;
  }

  size_t size() const {
    return _str->length();
  }

  typedef storage_policies::store_by_copy storage_policy;

 private:
  const ::String* _str;
};

template <>
struct IsString< ::String> : true_type {};

template <>
struct IsString< ::StringSumHelper> : true_type {};

inline ArduinoStringAdapter adaptString(const ::String& str) {
  return ArduinoStringAdapter(str);
}

}  // namespace ARDUINOJSON_NAMESPACE
