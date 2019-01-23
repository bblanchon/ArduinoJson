// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonDocument.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <size_t CAPACITY>
class StaticJsonDocument : public JsonDocument {
  static const size_t ACTUAL_CAPACITY =
      AddPadding<Max<1, CAPACITY>::value>::value;

 public:
  StaticJsonDocument() : JsonDocument(_buffer, ACTUAL_CAPACITY) {}

  StaticJsonDocument(const StaticJsonDocument& src)
      : JsonDocument(_buffer, ACTUAL_CAPACITY) {
    set(src);
  }

  template <typename T>
  StaticJsonDocument(const T& src,
                     typename enable_if<IsVisitable<T>::value>::type* = 0)
      : JsonDocument(_buffer, ACTUAL_CAPACITY) {
    set(src);
  }

  // disambiguate
  StaticJsonDocument(VariantRef src) : JsonDocument(_buffer, ACTUAL_CAPACITY) {
    set(src);
  }

  StaticJsonDocument operator=(const StaticJsonDocument& src) {
    set(src);
    return *this;
  }

  template <typename T>
  StaticJsonDocument operator=(const T& src) {
    set(src);
    return *this;
  }

 private:
  char _buffer[ACTUAL_CAPACITY];
};

}  // namespace ARDUINOJSON_NAMESPACE
