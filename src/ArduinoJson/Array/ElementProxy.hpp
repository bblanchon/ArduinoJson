// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Variant/VariantRefBase.hpp>

namespace ARDUINOJSON_NAMESPACE {

// A proxy class to get or set an element of an array.
// https://arduinojson.org/v6/api/jsonarray/subscript/
template <typename TUpstream>
class ElementProxy : public VariantRefBase<ElementProxy<TUpstream> >,
                     public VariantOperators<ElementProxy<TUpstream> > {
  friend class VariantAttorney;

 public:
  ElementProxy(TUpstream upstream, size_t index)
      : _upstream(upstream), _index(index) {}

  ElementProxy(const ElementProxy& src)
      : _upstream(src._upstream), _index(src._index) {}

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
    return VariantAttorney::getPool(_upstream);
  }

  FORCE_INLINE VariantData* getData() const {
    return variantGetElement(VariantAttorney::getData(_upstream), _index);
  }

  FORCE_INLINE VariantData* getOrCreateData() const {
    return variantGetOrAddElement(VariantAttorney::getOrCreateData(_upstream),
                                  _index, VariantAttorney::getPool(_upstream));
  }

  TUpstream _upstream;
  size_t _index;
};

}  // namespace ARDUINOJSON_NAMESPACE
