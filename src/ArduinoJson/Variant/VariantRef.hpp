// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Variant/VariantRefBase.hpp>

namespace ARDUINOJSON_NAMESPACE {

class VariantDataSource {
 public:
  VariantDataSource() : _data(0), _pool(0) {}

  VariantDataSource(MemoryPool* pool, VariantData* data)
      : _data(data), _pool(pool) {}

  FORCE_INLINE MemoryPool* getPool() const {
    return _pool;
  }

  FORCE_INLINE VariantData* getData() const {
    return _data;
  }

  FORCE_INLINE VariantData* getOrCreateData() const {
    return _data;
  }

 private:
  VariantData* _data;
  MemoryPool* _pool;
};

class VariantRef : public VariantRefBase<VariantDataSource>,
                   public VariantOperators<VariantRef> {
 public:
  VariantRef() : VariantRefBase<VariantDataSource>(VariantDataSource()) {}

  VariantRef(MemoryPool* pool, VariantData* data)
      : VariantRefBase<VariantDataSource>(VariantDataSource(pool, data)) {}
};

template <>
struct Converter<VariantRef> : private VariantAttorney {
  static void toJson(VariantRef src, VariantRef dst) {
    variantCopyFrom(getData(dst), getData(src), getPool(dst));
  }

  static VariantRef fromJson(VariantRef src) {
    return src;
  }

  static InvalidConversion<VariantConstRef, VariantRef> fromJson(
      VariantConstRef);

  static bool checkJson(VariantRef src) {
    VariantData* data = getData(src);
    return !!data;
  }

  static bool checkJson(VariantConstRef) {
    return false;
  }
};

template <>
struct Converter<VariantConstRef> : private VariantAttorney {
  static void toJson(VariantConstRef src, VariantRef dst) {
    variantCopyFrom(getData(dst), getData(src), getPool(dst));
  }

  static VariantConstRef fromJson(VariantConstRef src) {
    return VariantConstRef(getData(src));
  }

  static bool checkJson(VariantConstRef src) {
    const VariantData* data = getData(src);
    return !!data;
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
