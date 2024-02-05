// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Variant/VariantRefBase.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

// A proxy class to get or set an element of an array.
// https://arduinojson.org/v7/api/jsonarray/subscript/
template <typename TUpstream>
class ElementProxy : public VariantRefBase<ElementProxy<TUpstream>>,
                     public VariantOperators<ElementProxy<TUpstream>> {
  friend class VariantAttorney;

 public:
  ElementProxy(TUpstream upstream, size_t index)
      : upstream_(upstream), index_(index) {}

  ElementProxy(const ElementProxy& src)
      : upstream_(src.upstream_), index_(src.index_) {}

  ElementProxy& operator=(const ElementProxy& src) {
    this->set(src);
    return *this;
  }

  template <typename T>
  ElementProxy& operator=(const T& src) {
    this->set(src);
    return *this;
  }

  template <typename T>
  ElementProxy& operator=(T* src) {
    this->set(src);
    return *this;
  }

 private:
  ResourceManager* getResourceManager() const {
    return VariantAttorney::getResourceManager(upstream_);
  }

  FORCE_INLINE VariantData* getData() const {
    return VariantData::getElement(
        VariantAttorney::getData(upstream_), index_,
        VariantAttorney::getResourceManager(upstream_));
  }

  VariantData* getOrCreateData() const {
    auto data = VariantAttorney::getOrCreateData(upstream_);
    if (!data)
      return nullptr;
    return data->getOrAddElement(
        index_, VariantAttorney::getResourceManager(upstream_));
  }

  TUpstream upstream_;
  size_t index_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
