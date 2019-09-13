// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

#include <string>

namespace ARDUINOJSON_NAMESPACE {

template <typename TString>
class StlStringAdapter {
 public:
  StlStringAdapter(const TString& str) : _str(&str) {}

  char* save(MemoryPool* pool) const {
    size_t n = _str->length() + 1;
    char* dup = pool->allocFrozenString(n);
    if (dup) memcpy(dup, _str->c_str(), n);
    return dup;
  }

  bool isNull() const {
    return false;
  }

  int8_t compare(const char* other) const {
    if (!other) return 1;
    return static_cast<int8_t>(_str->compare(other));
  }

  bool equals(const char* expected) const {
    if (!expected) return false;
    return *_str == expected;
  }

  const char* data() const {
    return _str->data();
  }

  size_t size() const {
    return _str->size();
  }

  bool isStatic() const {
    return false;
  }

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
