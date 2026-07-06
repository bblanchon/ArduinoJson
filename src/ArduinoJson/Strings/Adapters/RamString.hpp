// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <stddef.h>  // size_t
#include <string.h>  // strcmp

#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/attributes.hpp>
#include <ArduinoJson/Strings/StringAdapter.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename T>
struct IsChar
    : integral_constant<bool, is_integral<T>::value && sizeof(T) == 1> {};

class RamString {
 public:
  static const size_t typeSortKey = 2;

  RamString(const char* str, size_t sz) : str_(str), size_(sz) {
    ARDUINOJSON_ASSERT(size_ == sz);
  }

  bool isNull() const {
    return !str_;
  }

  size_t size() const {
    return size_;
  }

  char operator[](size_t i) const {
    ARDUINOJSON_ASSERT(str_ != 0);
    ARDUINOJSON_ASSERT(i <= size());
    return str_[i];
  }

  const char* data() const {
    return str_;
  }

 protected:
  const char* str_;
  size_t size_;
};

template <typename TChar>
struct StringAdapter<TChar*, enable_if_t<IsChar<TChar>::value>> {
  using AdaptedString = RamString;

  static AdaptedString adapt(const TChar* p) {
    auto str = reinterpret_cast<const char*>(p);
    return AdaptedString(str, str ? ::strlen(str) : 0);
  }
};

template <typename TChar>
struct SizedStringAdapter<TChar*, enable_if_t<IsChar<TChar>::value>> {
  using AdaptedString = RamString;

  static AdaptedString adapt(const TChar* p, size_t n) {
    return AdaptedString(reinterpret_cast<const char*>(p), n);
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
