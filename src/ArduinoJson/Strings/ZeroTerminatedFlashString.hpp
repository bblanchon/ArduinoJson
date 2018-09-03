// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

class ZeroTerminatedFlashString {
 public:
  ZeroTerminatedFlashString(const __FlashStringHelper* str) : _str(str) {}

  bool equals(const char* expected) const {
    const char* actual = reinterpret_cast<const char*>(_str);
    if (!actual || !expected) return actual == expected;
    return strcmp_P(expected, actual) == 0;
  }

  bool is_null() const {
    return !_str;
  }

  template <typename Buffer>
  const char* save(Buffer* memoryPool) const {
    if (!_str) return NULL;
    size_t n = size() + 1;  // copy the terminator
    void* dup = memoryPool->alloc(n);
    if (dup != NULL) memcpy_P(dup, (const char*)_str, n);
    return static_cast<const char*>(dup);
  }

  size_t size() const {
    return strlen_P(reinterpret_cast<const char*>(_str));
  }

 private:
  const __FlashStringHelper* _str;
};

inline ZeroTerminatedFlashString makeString(const __FlashStringHelper* str) {
  return ZeroTerminatedFlashString(str);
}

template <>
struct IsString<const __FlashStringHelper*> : true_type {};
}  // namespace Internals
}  // namespace ArduinoJson
