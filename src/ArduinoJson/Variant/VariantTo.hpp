// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

namespace ARDUINOJSON_NAMESPACE {
class ArrayRef;
class ObjectRef;
class VariantRef;

// A metafunction that returns the type of the value returned by
// VariantRef::to<T>()
template <typename T>
struct VariantTo {};

template <>
struct VariantTo<ArrayRef> {
  typedef ArrayRef type;
};
template <>
struct VariantTo<ObjectRef> {
  typedef ObjectRef type;
};
template <>
struct VariantTo<VariantRef> {
  typedef VariantRef type;
};

}  // namespace ARDUINOJSON_NAMESPACE
