// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "ConstRamStringWrapper.hpp"

namespace ARDUINOJSON_NAMESPACE {

class RamStringWrapper : public ConstRamStringWrapper {
 public:
  RamStringWrapper(const char* str) : ConstRamStringWrapper(str) {}

  char* save(MemoryPool* pool) const {
    if (!_str) return NULL;
    size_t n = size() + 1;
    char* dup = pool->allocFrozenString(n);
    if (dup) memcpy(dup, _str, n);
    return dup;
  }
};

template <typename TChar>
inline RamStringWrapper wrapString(const TChar* str) {
  return RamStringWrapper(reinterpret_cast<const char*>(str));
}

inline RamStringWrapper wrapString(char* str) {
  return RamStringWrapper(str);
}

template <typename TChar>
struct IsString<TChar*> {
  static const bool value = sizeof(TChar) == 1;
};

template <>
struct IsString<void*> {
  static const bool value = false;
};

}  // namespace ARDUINOJSON_NAMESPACE
