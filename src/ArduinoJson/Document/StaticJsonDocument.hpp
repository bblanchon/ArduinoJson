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

  StaticJsonDocument(const JsonDocument& src)
      : JsonDocument(_buffer, ACTUAL_CAPACITY) {
    copy(src);
  }

  StaticJsonDocument operator=(const JsonDocument& src) {
    copy(src);
    return *this;
  }

 private:
  char _buffer[ACTUAL_CAPACITY];
};

}  // namespace ARDUINOJSON_NAMESPACE
