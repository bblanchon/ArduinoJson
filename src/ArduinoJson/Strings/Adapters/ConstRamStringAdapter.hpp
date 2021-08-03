// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <stddef.h>  // size_t
#include <string.h>  // strcmp

#include <ArduinoJson/Polyfills/safe_strcmp.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>
#include <ArduinoJson/Strings/StringAdapter.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <>
class StringAdapter<const char*> {
 public:
  StringAdapter(const char* str = 0) : _str(str) {}

  int compare(const char* other) const {
    return safe_strcmp(_str, other);
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

template <int N>
class StringAdapter<const char[N]> : public StringAdapter<const char*> {
 public:
  StringAdapter(const char* s) : StringAdapter<const char*>(s) {}
};

}  // namespace ARDUINOJSON_NAMESPACE
