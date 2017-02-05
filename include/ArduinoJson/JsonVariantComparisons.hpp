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

namespace TypeTraits {
template <typename T>
struct IsVariant : IsBaseOf<JsonVariantBase<T>, T> {};

template <typename T, typename Enable = void>
struct IsString {
  static const bool value = false;
};

template <typename T>
struct IsString<T, typename TypeTraits::EnableIf<
                       Internals::StringTraits<T>::has_equals>::type> {
  static const bool value = Internals::StringTraits<T>::has_equals;
};
}

template <typename T1, typename T2, typename Enable = void>
struct JsonVariantComparer {};

template <typename TVariant, typename TString>
struct JsonVariantComparer<
    TVariant, TString,
    typename TypeTraits::EnableIf<TypeTraits::IsVariant<TVariant>::value &&
                                  TypeTraits::IsString<TString>::value>::type> {
  static bool equals(const TVariant &variant, const TString &comparand) {
    const char *value = variant.template as<const char *>();
    return Internals::StringTraits<TString>::equals(comparand, value);
  }
};

template <typename TString, typename TVariant>
struct JsonVariantComparer<
    TString, TVariant,
    typename TypeTraits::EnableIf<TypeTraits::IsVariant<TVariant>::value &&
                                  TypeTraits::IsString<TString>::value>::type> {
  static bool equals(const TString &comparand, const TVariant &variant) {
    const char *value = variant.template as<const char *>();
    return Internals::StringTraits<TString>::equals(comparand, value);
  }
};

template <typename TVariant, typename TComparand>
struct JsonVariantComparer<
    TVariant, TComparand, typename TypeTraits::EnableIf<
                              TypeTraits::IsVariant<TVariant>::value &&
                              !TypeTraits::IsVariant<TComparand>::value &&
                              !TypeTraits::IsString<TComparand>::value>::type> {
  static bool equals(const TVariant &variant, const TComparand &comparand) {
    return variant.template as<TComparand>() == comparand;
  }
};

template <typename TComparand, typename TVariant>
struct JsonVariantComparer<
    TComparand, TVariant, typename TypeTraits::EnableIf<
                              TypeTraits::IsVariant<TVariant>::value &&
                              !TypeTraits::IsVariant<TComparand>::value &&
                              !TypeTraits::IsString<TComparand>::value>::type> {
  static bool equals(const TComparand &comparand, const TVariant &variant) {
    return variant.template as<TComparand>() == comparand;
  }
};

template <typename TVariant1, typename TVariant2>
struct JsonVariantComparer<TVariant1, TVariant2,
                           typename TypeTraits::EnableIf<
                               TypeTraits::IsVariant<TVariant1>::value &&
                               TypeTraits::IsVariant<TVariant2>::value>::type> {
  static bool equals(const TVariant1 &variant1, const TVariant2 &variant2) {
    return &variant1 == &variant2;  // TODO
  }
};

template <typename T1, typename T2>
inline bool operator==(const T1 &variant, const T2 &comparand) {
  return JsonVariantComparer<T1, T2>::equals(variant, comparand);
}

template <typename T1, typename T2>
inline bool operator!=(const T1 &variant, const T2 &comparand) {
  return !JsonVariantComparer<T1, T2>::equals(variant, comparand);
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
