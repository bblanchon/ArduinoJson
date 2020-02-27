// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Strings/IsString.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>

#include <string.h>  // strcmp

namespace ARDUINOJSON_NAMESPACE {

class SizedRamStringAdapter {
 public:
  SizedRamStringAdapter(const char* str, size_t n) : _str(str), _size(n) {}

  int compare(const char* other) const {
    return safe_strncmp(_str, other, _size);
  }

  bool equals(const char* expected) const {
    return compare(expected) == 0;
  }

  bool isNull() const {
    return !_str;
  }

  char* save(MemoryPool* pool) const {
    if (!_str)
      return NULL;
    char* dup = pool->allocFrozenString(_size);
    if (dup)
      memcpy(dup, _str, _size);
    return dup;
  }

  size_t size() const {
    return _size;
  }

  typedef storage_policy::store_by_copy storage_policy;

 private:
  const char* _str;
  size_t _size;
};

template <typename TChar>
inline SizedRamStringAdapter adaptString(const TChar* str, size_t size) {
  return SizedRamStringAdapter(reinterpret_cast<const char*>(str), size);
}

}  // namespace ARDUINOJSON_NAMESPACE
