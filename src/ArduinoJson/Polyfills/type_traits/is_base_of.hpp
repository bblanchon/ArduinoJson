// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

namespace ARDUINOJSON_NAMESPACE {

// A meta-function that returns true if Derived inherits from TBase is an
// integral type.
template <typename TBase, typename TDerived>
class is_base_of {
 protected:  // <- to avoid GCC's "all member functions in class are private"
  static int probe(const TBase*);
  static char probe(...);

 public:
  static const bool value =
      sizeof(probe(reinterpret_cast<TDerived*>(0))) == sizeof(int);
};
}  // namespace ARDUINOJSON_NAMESPACE
