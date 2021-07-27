// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>
#include <ArduinoJson/Strings/StringAdapter.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <>
class StringAdapter<const __FlashStringHelper*, true> {
 public:
  StringAdapter(const __FlashStringHelper* str, size_t sz)
      : _str(str), _size(sz) {}

  int compare(const char* other) const {
    if (!other && !_str)
      return 0;
    if (!_str)
      return -1;
    if (!other)
      return 1;
    return -strncmp_P(other, reinterpret_cast<const char*>(_str), _size);
  }

  bool isNull() const {
    return !_str;
  }

  void copyTo(char* p, size_t n) const {
    memcpy_P(p, reinterpret_cast<const char*>(_str), n);
  }

  size_t size() const {
    return _size;
  }

  typedef storage_policies::store_by_copy storage_policy;

 private:
  const __FlashStringHelper* _str;
  size_t _size;
};

}  // namespace ARDUINOJSON_NAMESPACE
