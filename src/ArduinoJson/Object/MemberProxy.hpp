// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Variant/VariantProxy.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TUpstream, typename TStringRef>
class MemberDataSource {
 public:
  FORCE_INLINE MemberDataSource(TUpstream upstream, TStringRef key)
      : _upstream(upstream), _key(key) {}

  FORCE_INLINE MemoryPool* getPool() const {
    return VariantAttorney::getPool(_upstream);
  }

  FORCE_INLINE VariantData* getData() const {
    return variantGetMember(VariantAttorney::getData(_upstream),
                            adaptString(_key));
  }

  FORCE_INLINE VariantData* getOrCreateData() const {
    return variantGetOrAddMember(VariantAttorney::getOrCreateData(_upstream),
                                 _key, VariantAttorney::getPool(_upstream));
  }

 private:
#if defined _MSC_VER && _MSC_VER <= 1800  // Visual Studio 2013 or below
  // Prevent "assignment operator could not be generated"
  MemberDataSource& operator=(const MemberDataSource&);
#endif

  TUpstream _upstream;
  TStringRef _key;
};

}  // namespace ARDUINOJSON_NAMESPACE
