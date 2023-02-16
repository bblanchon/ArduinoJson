// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <Arduino.h>

#include <ArduinoJson/Polyfills/pgmspace.hpp>
#include <ArduinoJson/Strings/StringAdapter.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class FlashString {
 public:
  static const size_t typeSortKey = 1;

  FlashString(const __FlashStringHelper* str, size_t sz)
      : _str(reinterpret_cast<const char*>(str)), _size(sz) {}

  bool isNull() const {
    return !_str;
  }

  char operator[](size_t i) const {
    ARDUINOJSON_ASSERT(_str != 0);
    ARDUINOJSON_ASSERT(i <= _size);
    return static_cast<char>(pgm_read_byte(_str + i));
  }

  size_t size() const {
    return _size;
  }

  friend bool stringEquals(FlashString a, SizedRamString b) {
    ARDUINOJSON_ASSERT(a.typeSortKey < b.typeSortKey);
    ARDUINOJSON_ASSERT(!a.isNull());
    ARDUINOJSON_ASSERT(!b.isNull());
    if (a.size() != b.size())
      return false;
    return ::memcmp_P(b.data(), a._str, a._size) == 0;
  }

  friend int stringCompare(FlashString a, SizedRamString b) {
    ARDUINOJSON_ASSERT(a.typeSortKey < b.typeSortKey);
    ARDUINOJSON_ASSERT(!a.isNull());
    ARDUINOJSON_ASSERT(!b.isNull());
    size_t minsize = a.size() < b.size() ? a.size() : b.size();
    int res = ::memcmp_P(b.data(), a._str, minsize);
    if (res)
      return -res;
    if (a.size() < b.size())
      return -1;
    if (a.size() > b.size())
      return 1;
    return 0;
  }

  friend void stringGetChars(FlashString s, char* p, size_t n) {
    ARDUINOJSON_ASSERT(s.size() <= n);
    ::memcpy_P(p, s._str, n);
  }

  StringStoragePolicy::Copy storagePolicy() const {
    return StringStoragePolicy::Copy();
  }

 private:
  const char* _str;
  size_t _size;
};

template <>
struct StringAdapter<const __FlashStringHelper*, void> {
  typedef FlashString AdaptedString;

  static AdaptedString adapt(const __FlashStringHelper* s) {
    return AdaptedString(s, s ? strlen_P(reinterpret_cast<const char*>(s)) : 0);
  }
};

template <>
struct SizedStringAdapter<const __FlashStringHelper*, void> {
  typedef FlashString AdaptedString;

  static AdaptedString adapt(const __FlashStringHelper* s, size_t n) {
    return AdaptedString(s, n);
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
