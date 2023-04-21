// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <stddef.h>  // size_t
#include <string.h>  // strcmp

#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>
#include <ArduinoJson/Strings/StringAdapter.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename T>
struct IsChar
    : integral_constant<bool, is_integral<T>::value && sizeof(T) == 1> {};

class ZeroTerminatedRamString {
 public:
  static const size_t typeSortKey = 3;

  ZeroTerminatedRamString(const char* str) : str_(str) {}

  bool isNull() const {
    return !str_;
  }

  size_t size() const {
    return str_ ? ::strlen(str_) : 0;
  }

  char operator[](size_t i) const {
    ARDUINOJSON_ASSERT(str_ != 0);
    ARDUINOJSON_ASSERT(i <= size());
    return str_[i];
  }

  const char* data() const {
    return str_;
  }

  friend int stringCompare(ZeroTerminatedRamString a,
                           ZeroTerminatedRamString b) {
    ARDUINOJSON_ASSERT(!a.isNull());
    ARDUINOJSON_ASSERT(!b.isNull());
    return ::strcmp(a.str_, b.str_);
  }

  friend bool stringEquals(ZeroTerminatedRamString a,
                           ZeroTerminatedRamString b) {
    return stringCompare(a, b) == 0;
  }

  StringStoragePolicy::Copy storagePolicy() const {
    return StringStoragePolicy::Copy();
  }

 protected:
  const char* str_;
};

template <typename TChar>
struct StringAdapter<TChar*, typename enable_if<IsChar<TChar>::value>::type> {
  typedef ZeroTerminatedRamString AdaptedString;

  static AdaptedString adapt(const TChar* p) {
    return AdaptedString(reinterpret_cast<const char*>(p));
  }
};

template <typename TChar, size_t N>
struct StringAdapter<TChar[N], typename enable_if<IsChar<TChar>::value>::type> {
  typedef ZeroTerminatedRamString AdaptedString;

  static AdaptedString adapt(const TChar* p) {
    return AdaptedString(reinterpret_cast<const char*>(p));
  }
};

class StaticStringAdapter : public ZeroTerminatedRamString {
 public:
  StaticStringAdapter(const char* str) : ZeroTerminatedRamString(str) {}

  StringStoragePolicy::Link storagePolicy() const {
    return StringStoragePolicy::Link();
  }
};

template <>
struct StringAdapter<const char*, void> {
  typedef StaticStringAdapter AdaptedString;

  static AdaptedString adapt(const char* p) {
    return AdaptedString(p);
  }
};

class SizedRamString {
 public:
  static const size_t typeSortKey = 2;

  SizedRamString(const char* str, size_t sz) : str_(str), size_(sz) {}

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

  StringStoragePolicy::Copy storagePolicy() const {
    return StringStoragePolicy::Copy();
  }

 protected:
  const char* str_;
  size_t size_;
};

template <typename TChar>
struct SizedStringAdapter<TChar*,
                          typename enable_if<IsChar<TChar>::value>::type> {
  typedef SizedRamString AdaptedString;

  static AdaptedString adapt(const TChar* p, size_t n) {
    return AdaptedString(reinterpret_cast<const char*>(p), n);
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
