// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <class T, typename = void>
struct is_std_string : false_type {};

template <class T>
struct is_std_string<
    T, enable_if_t<is_same<void, decltype(T().push_back('a'))>::value &&
                   is_same<T&, decltype(T().append(""))>::value>> : true_type {
};

template <typename TDestination>
class Writer<TDestination, enable_if_t<is_std_string<TDestination>::value>> {
 public:
  Writer(TDestination& str) : str_(&str) {
    str.clear();
  }

  size_t write(uint8_t c) {
    str_->push_back(static_cast<char>(c));
    return 1;
  }

  size_t write(const uint8_t* s, size_t n) {
    str_->append(reinterpret_cast<const char*>(s), n);
    return n;
  }

 private:
  TDestination* str_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
