// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "ZeroTerminatedRamStringConst.hpp"

namespace ARDUINOJSON_NAMESPACE {

class ZeroTerminatedRamString : public ZeroTerminatedRamStringConst {
 public:
  ZeroTerminatedRamString(const char* str)
      : ZeroTerminatedRamStringConst(str) {}

  template <typename TMemoryPool>
  char* save(TMemoryPool* memoryPool) const {
    if (!_str) return NULL;
    size_t n = size() + 1;
    char* dup = memoryPool->allocFrozenString(n);
    if (dup) memcpy(dup, _str, n);
    return dup;
  }
};

template <typename TChar>
inline ZeroTerminatedRamString makeString(const TChar* str) {
  return ZeroTerminatedRamString(reinterpret_cast<const char*>(str));
}

inline ZeroTerminatedRamString makeString(char* str) {
  return ZeroTerminatedRamString(str);
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
