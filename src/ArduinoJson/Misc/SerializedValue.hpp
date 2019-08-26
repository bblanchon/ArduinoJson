// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Strings/StringAdapters.hpp>

namespace ARDUINOJSON_NAMESPACE {

// A special type of data that can be used to insert pregenerated JSON portions.
template <typename T>
class SerializedValue {
 public:
  explicit SerializedValue(T str) : _str(str) {}
  operator T() const {
    return _str;
  }

  const char* data() const {
    return _str.c_str();
  }

  size_t size() const {
    // CAUTION: the old Arduino String doesn't have size()
    return _str.length();
  }

 private:
  T _str;
};

template <typename TChar>
class SerializedValue<TChar*> {
 public:
  explicit SerializedValue(TChar* p, size_t n) : _data(p), _size(n) {}
  operator TChar*() const {
    return _data;
  }

  TChar* data() const {
    return _data;
  }

  size_t size() const {
    return _size;
  }

 private:
  TChar* _data;
  size_t _size;
};

template <typename T>
inline SerializedValue<T> serialized(T str) {
  return SerializedValue<T>(str);
}

template <typename TChar>
inline SerializedValue<TChar*> serialized(TChar* p) {
  return SerializedValue<TChar*>(p, adaptString(p).size());
}

template <typename TChar>
inline SerializedValue<TChar*> serialized(TChar* p, size_t n) {
  return SerializedValue<TChar*>(p, n);
}
}  // namespace ARDUINOJSON_NAMESPACE
