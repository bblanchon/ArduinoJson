// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Variant/VariantRefBase.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// A reference to a value in a JsonDocument.
// https://arduinojson.org/v7/api/jsonvariant/
class JsonVariant : public detail::VariantRefBase<JsonVariant>,
                    public detail::VariantOperators<JsonVariant> {
  friend class detail::VariantAttorney;

 public:
  // Creates an unbound reference.
  JsonVariant() {}

  // INTERNAL USE ONLY
  JsonVariant(detail::VariantData* data, detail::ResourceManager* resources)
      : impl_(data, resources) {}

  // INTERNAL USE ONLY
  JsonVariant(detail::VariantImpl impl) : impl_(impl) {}

 private:
  detail::ResourceManager* getResourceManager() const {
    return impl_.resources();
  }

  detail::VariantData* getData() const {
    return impl_.data();
  }

  detail::VariantData* getOrCreateData() const {
    return impl_.data();
  }

  mutable detail::VariantImpl impl_;
};

namespace detail {
bool copyVariant(JsonVariant dst, JsonVariantConst src);
}

template <>
struct Converter<JsonVariant> : private detail::VariantAttorney {
  static bool toJson(JsonVariantConst src, JsonVariant dst) {
    return copyVariant(dst, src);
  }

  static JsonVariant fromJson(JsonVariant src) {
    return src;
  }

  static bool checkJson(JsonVariant src) {
    auto data = getData(src);
    return !!data;
  }
};

template <>
struct Converter<JsonVariantConst> : private detail::VariantAttorney {
  static bool toJson(JsonVariantConst src, JsonVariant dst) {
    return copyVariant(dst, src);
  }

  static JsonVariantConst fromJson(JsonVariantConst src) {
    return JsonVariantConst(getData(src), getResourceManager(src));
  }

  static bool checkJson(JsonVariantConst src) {
    auto data = getData(src);
    return !!data;
  }
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
