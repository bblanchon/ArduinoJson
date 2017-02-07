// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../JsonVariantBase.hpp"
#include "../StringTraits/StringTraits.hpp"
#include "../TypeTraits/EnableIf.hpp"

namespace ArduinoJson {
namespace Internals {
template <typename TLeft, typename TRight, typename Enable = void>
struct JsonVariantComparer {
  static bool equals(const TLeft &left, const TRight &right) {
    return &left == &right;
  }
};

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
  static bool equals(const TVariant1 &left, const TVariant2 &right) {
    if (left.template is<bool>() && right.template is<bool>())
      return left.template as<bool>() == right.template as<bool>();
    if (left.template is<JsonInteger>() && right.template is<JsonInteger>())
      return left.template as<JsonInteger>() ==
             right.template as<JsonInteger>();
    if (left.template is<JsonFloat>() && right.template is<JsonFloat>())
      return left.template as<JsonFloat>() == right.template as<JsonFloat>();
    if (left.template is<char *>() && right.template is<char *>())
      return strcmp(left.template as<char *>(), right.template as<char *>()) ==
             0;
    return false;
  }
};
}
}
