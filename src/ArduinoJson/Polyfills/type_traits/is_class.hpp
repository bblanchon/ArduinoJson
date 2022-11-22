// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include "declval.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename T>
struct is_class {
 protected:  // <- to avoid GCC's "all member functions in class are private"
  template <typename U>
  static int probe(void (U::*)(void));
  template <typename>
  static char probe(...);

 public:
  static const bool value = sizeof(probe<T>(0)) == sizeof(int);
};

}  // namespace ARDUINOJSON_NAMESPACE
