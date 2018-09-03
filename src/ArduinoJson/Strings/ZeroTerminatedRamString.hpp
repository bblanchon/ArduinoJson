// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

class ZeroTerminatedRamString {
 public:
  ZeroTerminatedRamString(const char* str) : _str(str) {}

  bool equals(const char* expected) const {
    const char* actual = reinterpret_cast<const char*>(_str);
    if (!actual || !expected) return actual == expected;
    return strcmp(actual, expected) == 0;
  }

  bool is_null() const {
    return !_str;
  }

  template <typename Buffer>
  const char* save(Buffer* memoryPool) const {
    if (!_str) return NULL;
    size_t n = size() + 1;
    void* dup = memoryPool->alloc(n);
    if (!dup) return NULL;
    memcpy(dup, _str, n);
    return static_cast<const char*>(dup);
  }

  size_t size() const {
    return strlen(reinterpret_cast<const char*>(_str));
  }

 private:
  const char* _str;
};

template <typename TChar>
inline ZeroTerminatedRamString makeString(const TChar* str) {
  return ZeroTerminatedRamString(reinterpret_cast<const char*>(str));
}

template <typename TChar>
struct IsString<TChar*> {
  static const bool value = sizeof(TChar) == 1;
};

}  // namespace Internals
}  // namespace ArduinoJson
