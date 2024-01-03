// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Variant/JsonVariant.hpp>
#include <ArduinoJson/Variant/VariantAttorney.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

namespace DeserializationOption {
class Filter {
 public:
#if ARDUINOJSON_AUTO_SHRINK
  explicit Filter(JsonDocument& doc) : variant_(doc) {
    doc.shrinkToFit();
  }
#endif

  explicit Filter(JsonVariantConst variant) : variant_(variant) {}

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
