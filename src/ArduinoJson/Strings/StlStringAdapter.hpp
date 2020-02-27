// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Strings/IsString.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>

#include <string>

namespace ARDUINOJSON_NAMESPACE {

template <typename TString>
class StlStringAdapter {
 public:
  StlStringAdapter(const TString& str) : _str(&str) {}

  char* save(MemoryPool* pool) const {
    size_t n = _str->length() + 1;
    char* dup = pool->allocFrozenString(n);
    if (dup)
      memcpy(dup, _str->c_str(), n);
    return dup;
  }

  bool isNull() const {
    return false;
  }

  int compare(const char* other) const {
    if (!other)
      return 1;
    return _str->compare(other);
  }

  bool equals(const char* expected) const {
    if (!expected)
      return false;
    return *_str == expected;
  }

  size_t size() const {
    return _str->size();
  }

  typedef storage_policy::store_by_copy storage_policy;

 private:
  const TString* _str;
};

template <typename TCharTraits, typename TAllocator>
struct IsString<std::basic_string<char, TCharTraits, TAllocator> > : true_type {
};

template <typename TCharTraits, typename TAllocator>
inline StlStringAdapter<std::basic_string<char, TCharTraits, TAllocator> >
adaptString(const std::basic_string<char, TCharTraits, TAllocator>& str) {
  return StlStringAdapter<std::basic_string<char, TCharTraits, TAllocator> >(
      str);
}

}  // namespace ARDUINOJSON_NAMESPACE
