// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>
#include <ArduinoJson/Strings/StringAdapter.hpp>

#include <string.h>  // strcmp

namespace ARDUINOJSON_NAMESPACE {

template <typename TChar>
class StringAdapter<TChar*, true> {
 public:
  StringAdapter(const char* str, size_t n) : _str(str), _size(n) {}

  int compare(const char* other) const {
    return safe_strncmp(_str, other, _size);
  }

  bool isNull() const {
    return !_str;
  }

  void copyTo(char* p, size_t n) const {
    memcpy(p, _str, n);
  }

  size_t size() const {
    return _size;
  }

  typedef storage_policies::store_by_copy storage_policy;

 private:
  const char* _str;
  size_t _size;
};

}  // namespace ARDUINOJSON_NAMESPACE
