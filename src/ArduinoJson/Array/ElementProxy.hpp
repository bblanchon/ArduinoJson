// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
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

  friend class VariantRefBase<ElementProxy<TUpstream>>;

  template <typename, typename>
  friend class MemberProxy;

  template <typename>
  friend class ElementProxy;

 public:
  ElementProxy(TUpstream upstream, size_t index)
      : upstream_(upstream), index_(index) {}

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
  // clang-format off
  ElementProxy(const ElementProxy& src)  // Error here? See https://arduinojson.org/v7/proxy-non-copyable/
      : upstream_(src.upstream_), index_(src.index_) {}
  // clang-format on

  ResourceManager* getResourceManager() const {
    return VariantAttorney::getResourceManager(upstream_);
  }

  FORCE_INLINE VariantData* getData() const {
    return VariantAttorney::getVariantImpl(upstream_).getElement(index_);
  }

  VariantData* getOrCreateData() const {
    auto data = VariantAttorney::getOrCreateData(upstream_);
    auto resources = VariantAttorney::getResourceManager(upstream_);
    if (data && data->type == VariantType::Null)
      data->toArray();
    return VariantImpl(data, resources).getOrAddElement(index_);
  }

  TUpstream upstream_;
  size_t index_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
