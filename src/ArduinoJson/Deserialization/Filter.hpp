// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

namespace DeserializationOption {
class Filter {
 public:
  explicit Filter(JsonVariantConst v) : variant_(v) {}

  bool allow() const {
    return variant_;
  }

  bool allowArray() const {
    return variant_ == true || variant_.is<JsonArrayConst>();
  }

  bool allowObject() const {
    return variant_ == true || variant_.is<JsonObjectConst>();
  }

  bool allowValue() const {
    return variant_ == true;
  }

  template <typename TKey>
  Filter operator[](const TKey& key) const {
    if (variant_ == true)  // "true" means "allow recursively"
      return *this;
    JsonVariantConst member = variant_[key];
    return Filter(member.isNull() ? variant_["*"] : member);
  }

 private:
  JsonVariantConst variant_;
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
