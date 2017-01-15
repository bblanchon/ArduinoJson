// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "JsonVariantBase.hpp"
#include "StringTraits/StringTraits.hpp"
#include "TypeTraits/EnableIf.hpp"

namespace ArduinoJson {
template <typename TVariant, typename TComparand, typename Enable = void>
struct JsonVariantComparer {
  static bool equals(const TVariant &variant, const TComparand &comparand) {
    return variant.template as<TComparand>() == comparand;
  }
};

template <typename TVariant, typename TString>
struct JsonVariantComparer<
    TVariant, TString, typename TypeTraits::EnableIf<Internals::StringTraits<
                           TString>::has_equals>::type> {
  static bool equals(const TVariant &variant, const TString &comparand) {
    const char *value = variant.template as<const char *>();
    return Internals::StringTraits<TString>::equals(comparand, value);
  }
};

template <typename TImpl, typename TComparand>
inline bool operator==(const JsonVariantBase<TImpl> &variant,
                       TComparand comparand) {
  typedef JsonVariantBase<TImpl> TVariant;
  return JsonVariantComparer<TVariant, TComparand>::equals(variant, comparand);
}

template <typename TImpl, typename TComparand>
inline bool operator==(TComparand comparand,
                       const JsonVariantBase<TImpl> &variant) {
  typedef JsonVariantBase<TImpl> TVariant;
  return JsonVariantComparer<TVariant, TComparand>::equals(variant, comparand);
}

template <typename TImpl, typename TComparand>
inline bool operator!=(const JsonVariantBase<TImpl> &variant,
                       TComparand comparand) {
  typedef JsonVariantBase<TImpl> TVariant;
  return !JsonVariantComparer<TVariant, TComparand>::equals(variant, comparand);
}

template <typename TImpl, typename TComparand>
inline bool operator!=(TComparand comparand,
                       const JsonVariantBase<TImpl> &variant) {
  typedef JsonVariantBase<TImpl> TVariant;
  return !JsonVariantComparer<TVariant, TComparand>::equals(variant, comparand);
}

template <typename TImpl, typename TComparand>
inline bool operator<=(const JsonVariantBase<TImpl> &left, TComparand right) {
  return left.template as<TComparand>() <= right;
}

template <typename TImpl, typename TComparand>
inline bool operator<=(TComparand comparand,
                       const JsonVariantBase<TImpl> &variant) {
  return comparand <= variant.template as<TComparand>();
}

template <typename TImpl, typename TComparand>
inline bool operator>=(const JsonVariantBase<TImpl> &variant,
                       TComparand comparand) {
  return variant.template as<TComparand>() >= comparand;
}

template <typename TImpl, typename TComparand>
inline bool operator>=(TComparand comparand,
                       const JsonVariantBase<TImpl> &variant) {
  return comparand >= variant.template as<TComparand>();
}

template <typename TImpl, typename TComparand>
inline bool operator<(const JsonVariantBase<TImpl> &varian,
                      TComparand comparand) {
  return varian.template as<TComparand>() < comparand;
}

template <typename TImpl, typename TComparand>
inline bool operator<(TComparand comparand,
                      const JsonVariantBase<TImpl> &variant) {
  return comparand < variant.template as<TComparand>();
}

template <typename TImpl, typename TComparand>
inline bool operator>(const JsonVariantBase<TImpl> &variant,
                      TComparand comparand) {
  return variant.template as<TComparand>() > comparand;
}

template <typename TImpl, typename TComparand>
inline bool operator>(TComparand comparand,
                      const JsonVariantBase<TImpl> &variant) {
  return comparand > variant.template as<TComparand>();
}
}
