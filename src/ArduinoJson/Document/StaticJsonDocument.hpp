// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Document/JsonDocument.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// A JsonDocument with a memory pool on the stack.
template <size_t desiredCapacity>
class StaticJsonDocument : public JsonDocument {
  static const size_t capacity_ =
      detail::AddPadding<detail::Max<1, desiredCapacity>::value>::value;

 public:
  StaticJsonDocument() : JsonDocument(buffer_, capacity_) {}

  StaticJsonDocument(const StaticJsonDocument& src)
      : JsonDocument(buffer_, capacity_) {
    set(src);
  }

  template <typename T>
  StaticJsonDocument(
      const T& src,
      typename detail::enable_if<
          detail::is_convertible<T, JsonVariantConst>::value>::type* = 0)
      : JsonDocument(buffer_, capacity_) {
    set(src);
  }

  // disambiguate
  StaticJsonDocument(JsonVariant src) : JsonDocument(buffer_, capacity_) {
    set(src);
  }

  StaticJsonDocument& operator=(const StaticJsonDocument& src) {
    set(src);
    return *this;
  }

  template <typename T>
  StaticJsonDocument& operator=(const T& src) {
    set(src);
    return *this;
  }

  // Reclaims the memory leaked when removing and replacing values.
  // https://arduinojson.org/v6/api/jsondocument/garbagecollect/
  void garbageCollect() {
    StaticJsonDocument tmp(*this);
    set(tmp);
  }

 private:
  char buffer_[capacity_];
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
