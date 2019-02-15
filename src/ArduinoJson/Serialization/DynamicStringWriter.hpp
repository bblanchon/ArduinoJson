// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "../Polyfills/type_traits.hpp"

#if ARDUINOJSON_ENABLE_ARDUINO_STRING
#include <WString.h>
#endif

#if ARDUINOJSON_ENABLE_STD_STRING
#include <string>
#endif

namespace ARDUINOJSON_NAMESPACE {

template <typename>
struct IsWriteableString : false_type {};

// A Print implementation that allows to write in a String
template <typename TString>
class DynamicStringWriter {};

#if ARDUINOJSON_ENABLE_ARDUINO_STRING
template <>
struct IsWriteableString<String> : true_type {};

template <>
class DynamicStringWriter<String> {
 public:
  DynamicStringWriter(String &str) : _str(&str) {}

  size_t write(uint8_t c) {
    _str->operator+=(static_cast<char>(c));
    return 1;
  }

  size_t write(const uint8_t *s, size_t n) {
    // CAUTION: Arduino String doesn't have append()
    // and old version doesn't have size() either
    _str->reserve(_str->length() + n);
    while (n > 0) {
      _str->operator+=(static_cast<char>(*s++));
      n--;
    }
    return n;
  }

 private:
  String *_str;
};
#endif

#if ARDUINOJSON_ENABLE_STD_STRING
template <>
struct IsWriteableString<std::string> : true_type {};

template <>
class DynamicStringWriter<std::string> {
 public:
  DynamicStringWriter(std::string &str) : _str(&str) {}

  size_t write(uint8_t c) {
    _str->operator+=(static_cast<char>(c));
    return 1;
  }

  size_t write(const uint8_t *s, size_t n) {
    _str->append(reinterpret_cast<const char *>(s), n);
    return n;
  }

 private:
  std::string *_str;
};
#endif
}  // namespace ARDUINOJSON_NAMESPACE
