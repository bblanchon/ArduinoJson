// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include "declval.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename T>
struct is_class {
 protected:  // <- to avoid GCC's "all member functions in class are private"
  typedef char Yes[1];
  typedef char No[2];

  template <typename U>
  static Yes &probe(void (U::*)(void));
  template <typename>
  static No &probe(...);

 public:
  static const bool value = sizeof(probe<T>(0)) == sizeof(Yes);
};

}  // namespace ARDUINOJSON_NAMESPACE
