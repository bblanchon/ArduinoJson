// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/JsonVariantAs.hpp"
#include "Polyfills/attributes.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename TImpl>
class JsonVariantCasts {
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
}  // namespace Internals
}  // namespace ArduinoJson
