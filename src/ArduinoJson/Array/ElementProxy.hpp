// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Variant/VariantRefBase.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

// A proxy class to get or set an element of an array.
// https://arduinojson.org/v6/api/jsonarray/subscript/
template <typename TUpstream>
class ElementProxy : public VariantRefBase<ElementProxy<TUpstream>>,
                     public VariantOperators<ElementProxy<TUpstream>> {
  friend class VariantAttorney;

 public:
  ElementProxy(TUpstream upstream, size_t index)
      : upstream_(upstream), index_(index) {}

  ElementProxy(const ElementProxy& src)
      : upstream_(src.upstream_), index_(src.index_) {}

  FORCE_INLINE ElementProxy& operator=(const ElementProxy& src) {
    this->set(src);
    return *this;
  }

  template <typename T>
  FORCE_INLINE ElementProxy& operator=(const T& src) {
    this->set(src);
    return *this;
  }

  template <typename T>
  FORCE_INLINE ElementProxy& operator=(T* src) {
    this->set(src);
    return *this;
  }

 private:
  FORCE_INLINE MemoryPool* getPool() const {
    return VariantAttorney::getPool(upstream_);
  }

  FORCE_INLINE VariantData* getData() const {
    return variantGetElement(VariantAttorney::getData(upstream_), index_);
  }

  FORCE_INLINE VariantData* getOrCreateData() const {
    return variantGetOrAddElement(VariantAttorney::getOrCreateData(upstream_),
                                  index_, VariantAttorney::getPool(upstream_));
  }

  TUpstream upstream_;
  size_t index_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
