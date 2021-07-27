// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/pgmspace.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>
#include <ArduinoJson/Strings/StringAdapter.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <>
class StringAdapter<const __FlashStringHelper*> {
 public:
  StringAdapter(const __FlashStringHelper* str) : _str(str) {}

  int compare(const char* other) const {
    if (!other && !_str)
      return 0;
    if (!_str)
      return -1;
    if (!other)
      return 1;
    return -strcmp_P(other, reinterpret_cast<const char*>(_str));
  }

  bool isNull() const {
    return !_str;
  }

  void copyTo(char* p, size_t n) const {
    memcpy_P(p, reinterpret_cast<const char*>(_str), n);
  }

  size_t size() const {
    if (!_str)
      return 0;
    return strlen_P(reinterpret_cast<const char*>(_str));
  }

  typedef storage_policies::store_by_copy storage_policy;

 private:
  const __FlashStringHelper* _str;
};

}  // namespace ARDUINOJSON_NAMESPACE
