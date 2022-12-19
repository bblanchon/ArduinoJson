// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Variant/VariantRefBase.hpp>

namespace ARDUINOJSON_NAMESPACE {

// A proxy class to get or set a member of an object.
// https://arduinojson.org/v6/api/jsonobject/subscript/
template <typename TUpstream, typename TStringRef>
class MemberProxy
    : public VariantRefBase<MemberProxy<TUpstream, TStringRef> >,
      public VariantOperators<MemberProxy<TUpstream, TStringRef> > {
  friend class VariantAttorney;

 public:
  FORCE_INLINE MemberProxy(TUpstream upstream, TStringRef key)
      : _upstream(upstream), _key(key) {}

  MemberProxy(const MemberProxy& src)
      : _upstream(src._upstream), _key(src._key) {}

  FORCE_INLINE MemberProxy& operator=(const MemberProxy& src) {
    this->set(src);
    return *this;
  }

  template <typename T>
  FORCE_INLINE MemberProxy& operator=(const T& src) {
    this->set(src);
    return *this;
  }

  template <typename T>
  FORCE_INLINE MemberProxy& operator=(T* src) {
    this->set(src);
    return *this;
  }

 private:
  FORCE_INLINE MemoryPool* getPool() const {
    return VariantAttorney::getPool(_upstream);
  }

  FORCE_INLINE VariantData* getData() const {
    return variantGetMember(VariantAttorney::getData(_upstream),
                            adaptString(_key));
  }

  FORCE_INLINE VariantData* getOrCreateData() const {
    return variantGetOrAddMember(VariantAttorney::getOrCreateData(_upstream),
                                 adaptString(_key),
                                 VariantAttorney::getPool(_upstream));
  }

 private:
  TUpstream _upstream;
  TStringRef _key;
};

}  // namespace ARDUINOJSON_NAMESPACE
