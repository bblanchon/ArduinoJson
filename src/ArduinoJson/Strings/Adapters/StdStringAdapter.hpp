// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>
#include <ArduinoJson/Strings/StringAdapter.hpp>

#include <string>

namespace ARDUINOJSON_NAMESPACE {

template <typename TCharTraits, typename TAllocator>
class StringAdapter<std::basic_string<char, TCharTraits, TAllocator> > {
 public:
  typedef std::basic_string<char, TCharTraits, TAllocator> string_type;

  StringAdapter(const string_type& str) : _str(&str) {}

  void copyTo(char* p, size_t n) const {
    memcpy(p, _str->c_str(), n);
  }

  bool isNull() const {
    return false;
  }

  int compare(const char* other) const {
    if (!other)
      return 1;
    return _str->compare(other);
  }

  size_t size() const {
    return _str->size();
  }

  typedef storage_policies::store_by_copy storage_policy;

 private:
  const string_type* _str;
};

}  // namespace ARDUINOJSON_NAMESPACE
