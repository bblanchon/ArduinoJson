// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Object/MemberProxy.hpp>
#include <ArduinoJson/Variant/VariantRef.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TSource>
struct Reader<VariantProxy<TSource>, void> : Reader<char*, void> {
  explicit Reader(const VariantProxy<TSource>& x)
      : Reader<char*, void>(x.template as<const char*>()) {}
};

template <>
struct Reader<VariantRef, void> : Reader<char*, void> {
  explicit Reader(VariantRef x) : Reader<char*, void>(x.as<const char*>()) {}
};

template <>
struct Reader<VariantConstRef, void> : Reader<char*, void> {
  explicit Reader(VariantConstRef x)
      : Reader<char*, void>(x.as<const char*>()) {}
};
}  // namespace ARDUINOJSON_NAMESPACE
