// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Strings/StringTypes.hpp"

namespace ArduinoJson {

namespace Internals {
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
}  // namespace Internals

template <typename T>
inline Internals::SerializedValue<T> serialized(T str) {
  return Internals::SerializedValue<T>(str);
}

template <typename TChar>
inline Internals::SerializedValue<TChar*> serialized(TChar* p) {
  return Internals::SerializedValue<TChar*>(p, Internals::makeString(p).size());
}

template <typename TChar>
inline Internals::SerializedValue<TChar*> serialized(TChar* p, size_t n) {
  return Internals::SerializedValue<TChar*>(p, n);
}
}  // namespace ArduinoJson
