// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Strings/StringAdapters.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// A special type of data that can be used to insert pregenerated JSON portions.
template <typename T>
class SerializedValue {
 public:
  explicit SerializedValue(T str) : str_(str) {}
  operator T() const {
    return str_;
  }

  const char* data() const {
    return str_.c_str();
  }

  size_t size() const {
    // CAUTION: the old Arduino String doesn't have size()
    return str_.length();
  }

 private:
  T str_;
};

template <typename TChar>
class SerializedValue<TChar*> {
 public:
  explicit SerializedValue(TChar* p, size_t n) : data_(p), size_(n) {}
  operator TChar*() const {
    return data_;
  }

  TChar* data() const {
    return data_;
  }

  size_t size() const {
    return size_;
  }

 private:
  TChar* data_;
  size_t size_;
};

template <typename T>
inline SerializedValue<T> serialized(T str) {
  return SerializedValue<T>(str);
}

template <typename TChar>
inline SerializedValue<TChar*> serialized(TChar* p) {
  return SerializedValue<TChar*>(p, detail::adaptString(p).size());
}

template <typename TChar>
inline SerializedValue<TChar*> serialized(TChar* p, size_t n) {
  return SerializedValue<TChar*>(p, n);
}

ARDUINOJSON_END_PUBLIC_NAMESPACE
