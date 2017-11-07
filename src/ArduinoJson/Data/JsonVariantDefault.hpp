// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2017
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

template <typename T>
struct JsonVariantDefault {
  static T get() {
    return T();
  }
};

template <typename T>
struct JsonVariantDefault<const T> : JsonVariantDefault<T> {};

template <typename T>
struct JsonVariantDefault<T&> : JsonVariantDefault<T> {};
}
}
