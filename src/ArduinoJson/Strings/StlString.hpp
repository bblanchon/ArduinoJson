// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <string>

namespace ARDUINOJSON_NAMESPACE {

class StlString {
 public:
  StlString(const std::string& str) : _str(&str) {}

  template <typename TMemoryPool>
  StringSlot* save(TMemoryPool* memoryPool) const {
    size_t n = _str->length() + 1;
    StringSlot* slot = memoryPool->allocFrozenString(n);
    if (slot) memcpy(slot->value, _str->c_str(), n);
    return slot;
  }

  bool isNull() const {
    return false;
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

 private:
  const std::string* _str;
};

template <>
struct IsString<std::string> : true_type {};

inline StlString makeString(const std::string& str) {
  return StlString(str);
}

}  // namespace ARDUINOJSON_NAMESPACE
