// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Strings/ConstRamStringAdapter.hpp>

namespace ARDUINOJSON_NAMESPACE {

class RamStringAdapter : public ConstRamStringAdapter {
 public:
  RamStringAdapter(const char* str) : ConstRamStringAdapter(str) {}

  char* save(MemoryPool* pool) const {
    if (!_str) return NULL;
    size_t n = size() + 1;
    char* dup = pool->allocFrozenString(n);
    if (dup) memcpy(dup, _str, n);
    return dup;
  }

  bool isStatic() const {
    return false;
  }
};

template <typename TChar>
inline RamStringAdapter adaptString(const TChar* str) {
  return RamStringAdapter(reinterpret_cast<const char*>(str));
}

inline RamStringAdapter adaptString(char* str) {
  return RamStringAdapter(str);
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
