// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include "declval.hpp"

#ifdef _MSC_VER
#pragma warning(push)
// conversion from 'T' to 'To', possible loss of data
#pragma warning(disable : 4244)
#endif

namespace ARDUINOJSON_NAMESPACE {

template <typename From, typename To>
struct is_convertible {
 protected:  // <- to avoid GCC's "all member functions in class are private"
  typedef char Yes[1];
  typedef char No[2];

  static Yes &probe(To);
  static No &probe(...);

 public:
  static const bool value = sizeof(probe(declval<From>())) == sizeof(Yes);
};

}  // namespace ARDUINOJSON_NAMESPACE

#ifdef _MSC_VER
#pragma warning(pop)
#endif
