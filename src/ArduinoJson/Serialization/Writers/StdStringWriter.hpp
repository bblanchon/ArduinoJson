// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>

#include <string>

namespace ARDUINOJSON_NAMESPACE {

template <class T>
struct is_std_string : false_type {};

template <class TCharTraits, class TAllocator>
struct is_std_string<std::basic_string<char, TCharTraits, TAllocator> >
    : true_type {};

template <typename TDestination>
class Writer<TDestination,
             typename enable_if<is_std_string<TDestination>::value>::type> {
 public:
  Writer(TDestination &str) : _str(&str) {}

  size_t write(uint8_t c) {
    _str->operator+=(static_cast<char>(c));
    return 1;
  }

  size_t write(const uint8_t *s, size_t n) {
    _str->append(reinterpret_cast<const char *>(s), n);
    return n;
  }

 private:
  TDestination *_str;
};
}  // namespace ARDUINOJSON_NAMESPACE
