// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "Data/JsonVariantComparer.hpp"

namespace ArduinoJson {

// operator==(TLeft left, TRight right)
//
template <typename TLeft, typename TRight>
inline typename TypeTraits::EnableIf<!TypeTraits::IsArray<TLeft>::value &&
                                         !TypeTraits::IsArray<TRight>::value,
                                     bool>::type
operator==(const TLeft &left, const TRight &right) {
  return Internals::JsonVariantComparer<TLeft, TRight>::equals(left, right);
}
// TLeft = const char*, const char[N], const FlashStringHelper*
template <typename TLeft, typename TRight>
inline bool operator==(const TLeft *left, const TRight &right) {
  return Internals::JsonVariantComparer<const TLeft *, TRight>::equals(left,
                                                                       right);
}
// TRight = const char*, const char[N], const FlashStringHelper*
template <typename TLeft, typename TRight>
inline bool operator==(const TLeft &left, const TRight *right) {
  return Internals::JsonVariantComparer<TLeft, const TRight *>::equals(left,
                                                                       right);
}

// operator!=(TLeft left, TRight right)
//
template <typename TLeft, typename TRight>
inline typename TypeTraits::EnableIf<!TypeTraits::IsArray<TLeft>::value &&
                                         !TypeTraits::IsArray<TRight>::value,
                                     bool>::type
operator!=(const TLeft &left, const TRight &right) {
  return !Internals::JsonVariantComparer<TLeft, TRight>::equals(left, right);
}
// TLeft = const char*, const char[N], const FlashStringHelper*
template <typename TLeft, typename TRight>
inline bool operator!=(const TLeft *left, const TRight &right) {
  return !Internals::JsonVariantComparer<const TLeft *, TRight>::equals(left,
                                                                        right);
}
// TRight = const char*, const char[N], const FlashStringHelper*
template <typename TLeft, typename TRight>
inline bool operator!=(const TLeft &left, const TRight *right) {
  return !Internals::JsonVariantComparer<TLeft, const TRight *>::equals(left,
                                                                        right);
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
