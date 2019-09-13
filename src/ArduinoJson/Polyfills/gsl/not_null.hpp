// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename T>
class not_null {
 public:
  explicit not_null(T ptr) : _ptr(ptr) {
    ARDUINOJSON_ASSERT(ptr != NULL);
  }

  T get() const {
    ARDUINOJSON_ASSERT(_ptr != NULL);
    return _ptr;
  }

 private:
  T _ptr;
};

template <typename T>
not_null<T> make_not_null(T ptr) {
  ARDUINOJSON_ASSERT(ptr != NULL);
  return not_null<T>(ptr);
}

}  // namespace ARDUINOJSON_NAMESPACE
