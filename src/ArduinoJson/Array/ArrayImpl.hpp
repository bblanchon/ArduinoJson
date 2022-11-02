// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ArrayRef.hpp>
#include <ArduinoJson/Object/ObjectRef.hpp>

namespace ARDUINOJSON_NAMESPACE {

inline ObjectRef ArrayRef::createNestedObject() const {
  return add().to<ObjectRef>();
}

template <typename TDataSource>
inline ArrayRef VariantRefBase<TDataSource>::createNestedArray() const {
  return add().template to<ArrayRef>();
}

template <typename TDataSource>
inline ObjectRef VariantRefBase<TDataSource>::createNestedObject() const {
  return add().template to<ObjectRef>();
}

template <typename TDataSource>
inline VariantProxy<ElementDataSource<VariantRefBase<TDataSource> > >
VariantRefBase<TDataSource>::operator[](size_t index) const {
  return VariantProxy<ElementDataSource<VariantRefBase<TDataSource> > >(
      ElementDataSource<VariantRefBase<TDataSource> >(*this, index));
}

}  // namespace ARDUINOJSON_NAMESPACE
