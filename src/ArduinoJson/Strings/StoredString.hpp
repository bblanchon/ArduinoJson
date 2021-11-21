// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

namespace ARDUINOJSON_NAMESPACE {

template <bool linked>
class StoredString {
 public:
  StoredString() : _data(0), _size(0) {}
  StoredString(const char* p, size_t n) : _data(p), _size(n) {}

  operator const char*() const {
    return _data;
  }

  const char* c_str() const {
    return _data;
  }

  size_t size() const {
    return _size;
  }

 private:
  const char* _data;
  size_t _size;
};

typedef StoredString<true> LinkedString;
typedef StoredString<false> CopiedString;

}  // namespace ARDUINOJSON_NAMESPACE
