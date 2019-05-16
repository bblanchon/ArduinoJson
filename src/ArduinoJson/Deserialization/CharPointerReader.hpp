// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

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
};

class SafeCharPointerReader {
  const char* _ptr;
  const char* _end;

 public:
  explicit SafeCharPointerReader(const char* ptr, size_t len)
      : _ptr(ptr ? ptr : reinterpret_cast<const char*>("")), _end(_ptr + len) {}

  int read() {
    if (_ptr < _end)
      return static_cast<unsigned char>(*_ptr++);
    else
      return -1;
  }
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
