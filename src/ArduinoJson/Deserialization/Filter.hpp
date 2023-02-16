// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

namespace DeserializationOption {
class Filter {
 public:
  explicit Filter(JsonVariantConst v) : _variant(v) {}

  bool allow() const {
    return _variant;
  }

  bool allowArray() const {
    return _variant == true || _variant.is<JsonArrayConst>();
  }

  bool allowObject() const {
    return _variant == true || _variant.is<JsonObjectConst>();
  }

  bool allowValue() const {
    return _variant == true;
  }

  template <typename TKey>
  Filter operator[](const TKey& key) const {
    if (_variant == true)  // "true" means "allow recursively"
      return *this;
    JsonVariantConst member = _variant[key];
    return Filter(member.isNull() ? _variant["*"] : member);
  }

 private:
  JsonVariantConst _variant;
};
}  // namespace DeserializationOption

namespace detail {
struct AllowAllFilter {
  bool allow() const {
    return true;
  }

  bool allowArray() const {
    return true;
  }

  bool allowObject() const {
    return true;
  }

  bool allowValue() const {
    return true;
  }

  template <typename TKey>
  AllowAllFilter operator[](const TKey&) const {
    return AllowAllFilter();
  }
};
}  // namespace detail

ARDUINOJSON_END_PUBLIC_NAMESPACE
