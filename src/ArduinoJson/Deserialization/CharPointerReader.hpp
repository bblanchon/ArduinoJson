// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

namespace ARDUINOJSON_NAMESPACE {

class UnsafeCharPointerReader {
  const char* _ptr;

 public:
  explicit UnsafeCharPointerReader(const char* ptr)
      : _ptr(ptr ? ptr : reinterpret_cast<const char*>("")) {}

  char read() {
    return static_cast<char>(*_ptr++);
  }

  bool ended() const {
    // we cannot know, that's why it's unsafe
    return false;
  }
};

class SafeCharPointerReader {
  const char* _ptr;
  const char* _end;

 public:
  explicit SafeCharPointerReader(const char* ptr, size_t len)
      : _ptr(ptr ? ptr : reinterpret_cast<const char*>("")), _end(_ptr + len) {}

  char read() {
    return static_cast<char>(*_ptr++);
  }

  bool ended() const {
    return _ptr == _end;
  }
};

template <typename TChar>
inline UnsafeCharPointerReader makeReader(TChar* input) {
  return UnsafeCharPointerReader(reinterpret_cast<const char*>(input));
}

template <typename TChar>
inline SafeCharPointerReader makeReader(TChar* input, size_t n) {
  return SafeCharPointerReader(reinterpret_cast<const char*>(input), n);
}

#if ARDUINOJSON_ENABLE_ARDUINO_STRING
inline SafeCharPointerReader makeReader(const ::String& input) {
  return SafeCharPointerReader(input.c_str(), input.length());
}
#endif

}  // namespace ARDUINOJSON_NAMESPACE
