// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <WString.h>

namespace ArduinoJson {
namespace Internals {

class ArduinoString {
 public:
  ArduinoString(const ::String& str) : _str(&str) {}

  template <typename Buffer>
  const char* save(Buffer* memoryPool) const {
    if (is_null()) return NULL;
    size_t n = _str->length() + 1;
    void* dup = memoryPool->alloc(n);
    if (dup != NULL) memcpy(dup, _str->c_str(), n);
    return static_cast<const char*>(dup);
  }

  bool is_null() const {
    // Arduino's String::c_str() can return NULL
    return !_str->c_str();
  }

  bool equals(const char* expected) const {
    // Arduino's String::c_str() can return NULL
    const char* actual = _str->c_str();
    if (!actual || !expected) return actual == expected;
    return 0 == strcmp(actual, expected);
  }

  const char* data() const {
    return _str->c_str();
  }

  size_t size() const {
    return _str->length();
  }

 private:
  const ::String* _str;
};

template <>
struct IsString< ::String> : true_type {};

template <>
struct IsString< ::StringSumHelper> : true_type {};

inline ArduinoString makeString(const ::String& str) {
  return ArduinoString(str);
}

}  // namespace Internals
}  // namespace ArduinoJson
