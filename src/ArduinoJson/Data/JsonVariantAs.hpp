// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ARDUINOJSON_NAMESPACE {

// A metafunction that returns the type of the value returned by
// JsonVariant::as<T>()
template <typename T>
struct JsonVariantAs {
  typedef T type;
};

template <>
struct JsonVariantAs<char*> {
  typedef const char* type;
};

}  // namespace ARDUINOJSON_NAMESPACE
