// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#if ARDUINOJSON_ENABLE_PROGMEM

namespace ArduinoJson {
namespace Internals {
template <>
struct StringTraits<const __FlashStringHelper*, void> {
  static bool equals(const __FlashStringHelper* str, const char* expected) {
    const char* actual = reinterpret_cast<const char*>(str);
    if (!actual || !expected) return actual == expected;
    return strcmp_P(expected, actual) == 0;
  }

  static bool is_null(const __FlashStringHelper* str) {
    return !str;
  }

  typedef const char* duplicate_t;

  template <typename Buffer>
  static duplicate_t duplicate(const __FlashStringHelper* str, Buffer* buffer) {
    if (!str) return NULL;
    size_t size = strlen_P((const char*)str) + 1;
    void* dup = buffer->alloc(size);
    if (dup != NULL) memcpy_P(dup, (const char*)str, size);
    return static_cast<duplicate_t>(dup);
  }

  static const bool has_append = false;
  static const bool has_equals = true;
  static const bool should_duplicate = true;
};
}  // namespace Internals
}  // namespace ArduinoJson

#endif
