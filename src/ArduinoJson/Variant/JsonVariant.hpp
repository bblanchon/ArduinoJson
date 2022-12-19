// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Variant/VariantRefBase.hpp>

namespace ARDUINOJSON_NAMESPACE {

// A reference to a value in a JsonDocument.
// https://arduinojson.org/v6/api/jsonvariant/
class JsonVariant : public VariantRefBase<JsonVariant>,
                    public VariantOperators<JsonVariant> {
  friend class VariantAttorney;

 public:
  // Creates an unbound reference.
  JsonVariant() : _data(0), _pool(0) {}

  // INTERNAL USE ONLY
  JsonVariant(MemoryPool* pool, VariantData* data) : _data(data), _pool(pool) {}

 private:
  FORCE_INLINE MemoryPool* getPool() const {
    return _pool;
  }

  FORCE_INLINE VariantData* getData() const {
    return _data;
  }

  FORCE_INLINE VariantData* getOrCreateData() const {
    return _data;
  }

  VariantData* _data;
  MemoryPool* _pool;
};

template <>
struct Converter<JsonVariant> : private VariantAttorney {
  static void toJson(JsonVariant src, JsonVariant dst) {
    variantCopyFrom(getData(dst), getData(src), getPool(dst));
  }

  static JsonVariant fromJson(JsonVariant src) {
    return src;
  }

  static InvalidConversion<JsonVariantConst, JsonVariant> fromJson(
      JsonVariantConst);

  static bool checkJson(JsonVariant src) {
    VariantData* data = getData(src);
    return !!data;
  }

  static bool checkJson(JsonVariantConst) {
    return false;
  }
};

template <>
struct Converter<JsonVariantConst> : private VariantAttorney {
  static void toJson(JsonVariantConst src, JsonVariant dst) {
    variantCopyFrom(getData(dst), getData(src), getPool(dst));
  }

  static JsonVariantConst fromJson(JsonVariantConst src) {
    return JsonVariantConst(getData(src));
  }

  static bool checkJson(JsonVariantConst src) {
    const VariantData* data = getData(src);
    return !!data;
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
