// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
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
  JsonVariant() : data_(0), resources_(0) {}

  // INTERNAL USE ONLY
  JsonVariant(detail::VariantData* data, detail::ResourceManager* resources)
      : data_(data), resources_(resources) {}

 private:
  detail::ResourceManager* getResourceManager() const {
    return resources_;
  }

  detail::VariantData* getData() const {
    return data_;
  }

  detail::VariantData* getOrCreateData() const {
    return data_;
  }

  detail::VariantData* data_;
  detail::ResourceManager* resources_;
};

namespace detail {
bool copyVariant(JsonVariant dst, JsonVariantConst src);
}

template <>
struct Converter<JsonVariant> : private detail::VariantAttorney {
  static void toJson(JsonVariantConst src, JsonVariant dst) {
    copyVariant(dst, src);
  }

  static JsonVariant fromJson(JsonVariant src) {
    return src;
  }

  static detail::InvalidConversion<JsonVariantConst, JsonVariant> fromJson(
      JsonVariantConst);

  static bool checkJson(JsonVariant src) {
    auto data = getData(src);
    return !!data;
  }

  static bool checkJson(JsonVariantConst) {
    return false;
  }
};

template <>
struct Converter<JsonVariantConst> : private detail::VariantAttorney {
  static void toJson(JsonVariantConst src, JsonVariant dst) {
    copyVariant(dst, src);
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
