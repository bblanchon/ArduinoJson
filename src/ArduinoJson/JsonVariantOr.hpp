// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/JsonVariantAs.hpp"
#include "Polyfills/attributes.hpp"
#include "Polyfills/type_traits.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename TImpl>
class JsonVariantOr {
 public:
  // Returns the default value if the JsonVariant is undefined of incompatible
  template <typename T>
  typename enable_if<!is_integral<T>::value, T>::type operator|(
      const T &defaultValue) const {
    if (impl()->template is<T>())
      return impl()->template as<T>();
    else
      return defaultValue;
  }

  // Returns the default value if the JsonVariant is undefined of incompatible
  // Special case for string: null is treated as undefined
  const char *operator|(const char *defaultValue) const {
    const char *value = impl()->template as<const char *>();
    return value ? value : defaultValue;
  }

  // Returns the default value if the JsonVariant is undefined of incompatible
  // Special case for integers: we also accept double
  template <typename Integer>
  typename enable_if<is_integral<Integer>::value, Integer>::type operator|(
      const Integer &defaultValue) const {
    if (impl()->template is<double>())
      return impl()->template as<Integer>();
    else
      return defaultValue;
  }

 private:
  const TImpl *impl() const {
    return static_cast<const TImpl *>(this);
  }
};
}  // namespace Internals
}  // namespace ArduinoJson
