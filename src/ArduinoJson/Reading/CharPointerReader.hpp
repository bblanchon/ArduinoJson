// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

template <typename TChar>
class UnsafeCharPointerReader {
  const TChar* _ptr;

 public:
  explicit UnsafeCharPointerReader(const TChar* ptr)
      : _ptr(ptr ? ptr : reinterpret_cast<const TChar*>("")) {}

  char read() {
    return static_cast<char>(*_ptr++);
  }

  bool ended() const {
    // we cannot know
    return false;
  }
};

template <typename TChar>
class SafeCharPointerReader {
  const TChar* _ptr;
  const TChar* _end;

 public:
  explicit SafeCharPointerReader(const TChar* ptr, size_t len)
      : _ptr(ptr ? ptr : reinterpret_cast<const TChar*>("")),
        _end(_ptr + len) {}

  char read() {
    return static_cast<char>(*_ptr++);
  }

  bool ended() const {
    return _ptr == _end;
  }
};

template <typename TChar>
inline UnsafeCharPointerReader<TChar> makeReader(TChar* input) {
  return UnsafeCharPointerReader<TChar>(input);
}

template <typename TChar>
inline SafeCharPointerReader<TChar> makeReader(TChar* input, size_t n) {
  return SafeCharPointerReader<TChar>(input, n);
}

#if ARDUINOJSON_ENABLE_ARDUINO_STRING
inline SafeCharPointerReader<char> makeReader(const String& input) {
  return SafeCharPointerReader<char>(input.c_str(), input.length());
}
#endif

}  // namespace Internals
}  // namespace ArduinoJson
