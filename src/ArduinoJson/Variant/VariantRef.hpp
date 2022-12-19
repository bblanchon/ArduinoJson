// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Variant/VariantRefBase.hpp>

namespace ARDUINOJSON_NAMESPACE {

class VariantRef : public VariantRefBase<VariantRef>,
                   public VariantOperators<VariantRef> {
  friend class VariantAttorney;

 public:
  VariantRef() : _data(0), _pool(0) {}

  VariantRef(MemoryPool* pool, VariantData* data) : _data(data), _pool(pool) {}

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
struct Converter<VariantRef> : private VariantAttorney {
  static void toJson(VariantRef src, VariantRef dst) {
    variantCopyFrom(getData(dst), getData(src), getPool(dst));
  }

  static VariantRef fromJson(VariantRef src) {
    return src;
  }

  static InvalidConversion<JsonVariantConst, VariantRef> fromJson(
      JsonVariantConst);

  static bool checkJson(VariantRef src) {
    VariantData* data = getData(src);
    return !!data;
  }

  static bool checkJson(JsonVariantConst) {
    return false;
  }
};

template <>
struct Converter<JsonVariantConst> : private VariantAttorney {
  static void toJson(JsonVariantConst src, VariantRef dst) {
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
