// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Strings/IsString.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>

#include <string_view>

namespace ARDUINOJSON_NAMESPACE {

class StringViewAdapter {
 public:
  StringViewAdapter(std::string_view str) : _str(str) {}

  void copyTo(char* p, size_t n) const {
    memcpy(p, _str.data(), n);
  }

  bool isNull() const {
    return false;
  }

  int compare(const char* other) const {
    if (!other)
      return 1;
    return _str.compare(other);
  }

  bool equals(const char* expected) const {
    if (!expected)
      return false;
    return _str == expected;
  }

  size_t size() const {
    return _str.size();
  }

  typedef storage_policies::store_by_copy storage_policy;

 private:
  std::string_view _str;
};

template <>
struct IsString<std::string_view> : true_type {};

inline StringViewAdapter adaptString(const std::string_view& str) {
  return StringViewAdapter(str);
}

}  // namespace ARDUINOJSON_NAMESPACE
