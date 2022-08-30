// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Variant/VariantProxy.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TUpstream>
class ElementDataSource {
 public:
  ElementDataSource(TUpstream upstream, size_t index)
      : _upstream(upstream), _index(index) {}

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

 private:
#if defined _MSC_VER && _MSC_VER <= 1800  // Visual Studio 2013 or below
  // Prevent "assignment operator could not be generated"
  ElementDataSource& operator=(const ElementDataSource&);
#endif

  TUpstream _upstream;
  size_t _index;
};

}  // namespace ARDUINOJSON_NAMESPACE
