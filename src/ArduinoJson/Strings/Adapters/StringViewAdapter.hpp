// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>
#include <ArduinoJson/Strings/StringAdapter.hpp>

#include <string_view>

namespace ARDUINOJSON_NAMESPACE {

template <>
class StringAdapter<std::string_view> {
 public:
  StringAdapter(std::string_view str) : _str(str) {}

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

  size_t size() const {
    return _str.size();
  }

  typedef storage_policies::store_by_copy storage_policy;

 private:
  std::string_view _str;
};

}  // namespace ARDUINOJSON_NAMESPACE
