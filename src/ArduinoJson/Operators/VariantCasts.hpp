// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/attributes.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TImpl>
class VariantCasts {
 public:
  template <typename T>
  FORCE_INLINE operator T() const {
    return impl()->template as<T>();
  }

 private:
  const TImpl *impl() const {
    return static_cast<const TImpl *>(this);
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
