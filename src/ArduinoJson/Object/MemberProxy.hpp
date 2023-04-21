// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Variant/VariantRefBase.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

// A proxy class to get or set a member of an object.
// https://arduinojson.org/v6/api/jsonobject/subscript/
template <typename TUpstream, typename TStringRef>
class MemberProxy
    : public VariantRefBase<MemberProxy<TUpstream, TStringRef>>,
      public VariantOperators<MemberProxy<TUpstream, TStringRef>> {
  friend class VariantAttorney;

 public:
  FORCE_INLINE MemberProxy(TUpstream upstream, TStringRef key)
      : upstream_(upstream), key_(key) {}

  MemberProxy(const MemberProxy& src)
      : upstream_(src.upstream_), key_(src.key_) {}

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
    return VariantAttorney::getPool(upstream_);
  }

  FORCE_INLINE VariantData* getData() const {
    return variantGetMember(VariantAttorney::getData(upstream_),
                            adaptString(key_));
  }

  FORCE_INLINE VariantData* getOrCreateData() const {
    return variantGetOrAddMember(VariantAttorney::getOrCreateData(upstream_),
                                 adaptString(key_),
                                 VariantAttorney::getPool(upstream_));
  }

 private:
  TUpstream upstream_;
  TStringRef key_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
