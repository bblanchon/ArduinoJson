// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Variant/VariantOperators.hpp>
#include <ArduinoJson/Variant/VariantRefBase.hpp>

#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 4522)
#endif

namespace ARDUINOJSON_NAMESPACE {

template <typename TDataSource>
class VariantProxy : public VariantRefBase<TDataSource>,
                     public VariantOperators<VariantProxy<TDataSource> > {
 public:
  explicit FORCE_INLINE VariantProxy(TDataSource source)
      : VariantRefBase<TDataSource>(source) {}

  // Copy-constructor required because of user-defined copy-assignment
  // operator
  FORCE_INLINE VariantProxy(const VariantProxy& src)
      : VariantRefBase<TDataSource>(src) {}

  FORCE_INLINE VariantProxy& operator=(const VariantProxy& src) {
    this->set(src);
    return *this;
  }

  template <typename T>
  FORCE_INLINE VariantProxy& operator=(const T& src) {
    this->set(src);
    return *this;
  }

  template <typename T>
  FORCE_INLINE VariantProxy& operator=(T* src) {
    this->set(src);
    return *this;
  }
};

}  // namespace ARDUINOJSON_NAMESPACE

#ifdef _MSC_VER
#  pragma warning(pop)
#endif
