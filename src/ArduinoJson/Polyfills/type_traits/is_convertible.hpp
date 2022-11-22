// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include "declval.hpp"

#ifdef _MSC_VER
#  pragma warning(push)
// conversion from 'T' to 'To', possible loss of data
#  pragma warning(disable : 4244)
#endif

// clang-format off
#ifdef __ICCARM__
// Suppress IAR Compiler Warning[Pa093]: implicit conversion from floating point to integer
#pragma diag_suppress=Pa093
#endif
// clang-format on

namespace ARDUINOJSON_NAMESPACE {

template <typename From, typename To>
struct is_convertible {
 protected:  // <- to avoid GCC's "all member functions in class are private"
  static int probe(To);
  static char probe(...);

  static From& _from;

 public:
  static const bool value = sizeof(probe(_from)) == sizeof(int);
};

}  // namespace ARDUINOJSON_NAMESPACE

#ifdef _MSC_VER
#  pragma warning(pop)
#endif

// clang-format off
#ifdef __ICCARM__
#pragma diag_default=Pa093
#endif
// clang-format on
