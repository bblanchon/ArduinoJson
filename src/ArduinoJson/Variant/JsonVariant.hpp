// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Variant/VariantRefBase.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// A reference to a value in a JsonDocument.
// https://arduinojson.org/v6/api/jsonvariant/
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
  FORCE_INLINE detail::ResourceManager* getResourceManager() const {
    return resources_;
  }

  FORCE_INLINE detail::VariantData* getData() const {
    return data_;
  }

  FORCE_INLINE detail::VariantData* getOrCreateData() const {
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
  static void toJson(JsonVariant src, JsonVariant dst) {
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

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TDerived>
inline JsonVariant VariantRefBase<TDerived>::add() const {
  return JsonVariant(
      detail::VariantData::addElement(getOrCreateData(), getResourceManager()),
      getResourceManager());
}

template <typename TDerived>
inline JsonVariant VariantRefBase<TDerived>::getVariant() const {
  return JsonVariant(getData(), getResourceManager());
}

template <typename TDerived>
inline JsonVariant VariantRefBase<TDerived>::getOrCreateVariant() const {
  return JsonVariant(getOrCreateData(), getResourceManager());
}

template <typename TDerived>
template <typename T>
typename enable_if<is_same<T, JsonVariant>::value, JsonVariant>::type
VariantRefBase<TDerived>::to() const {
  auto data = getOrCreateData();
  auto resources = getResourceManager();
  detail::VariantData::setNull(data, resources);
  return JsonVariant(data, resources);
}

template <typename TDerived>
inline void convertToJson(const VariantRefBase<TDerived>& src,
                          JsonVariant dst) {
  dst.set(src.template as<JsonVariantConst>());
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
