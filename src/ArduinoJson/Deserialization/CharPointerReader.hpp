// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Deserialization/IteratorReader.hpp>
#include <ArduinoJson/Namespace.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename T>
struct IsCharOrVoid {
  static const bool value =
      is_same<T, void>::value || is_same<T, char>::value ||
      is_same<T, unsigned char>::value || is_same<T, signed char>::value;
};

template <typename T>
struct IsCharOrVoid<const T> : IsCharOrVoid<T> {};

class UnsafeCharPointerReader {
  const char* _ptr;

 public:
  explicit UnsafeCharPointerReader(const char* ptr)
      : _ptr(ptr ? ptr : reinterpret_cast<const char*>("")) {}

  int read() {
    return static_cast<unsigned char>(*_ptr++);
  }

  size_t readBytes(char* buffer, size_t length) {
    for (size_t i = 0; i < length; i++) buffer[i] = *_ptr++;
    return length;
  }
};

class SafeCharPointerReader : public IteratorReader<const char*> {
 public:
  explicit SafeCharPointerReader(const char* ptr, size_t len)
      : IteratorReader<const char*>(ptr, ptr + len) {}
};

template <typename TChar>
inline typename enable_if<IsCharOrVoid<TChar>::value,
                          UnsafeCharPointerReader>::type
makeReader(TChar* input) {
  return UnsafeCharPointerReader(reinterpret_cast<const char*>(input));
}

template <typename TChar>
inline
    typename enable_if<IsCharOrVoid<TChar>::value, SafeCharPointerReader>::type
    makeReader(TChar* input, size_t n) {
  return SafeCharPointerReader(reinterpret_cast<const char*>(input), n);
}

#if ARDUINOJSON_ENABLE_ARDUINO_STRING
inline SafeCharPointerReader makeReader(const ::String& input) {
  return SafeCharPointerReader(input.c_str(), input.length());
}
#endif

}  // namespace ARDUINOJSON_NAMESPACE
