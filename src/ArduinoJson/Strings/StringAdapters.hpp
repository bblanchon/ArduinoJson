// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Strings/Adapters/JsonString.hpp>
#include <ArduinoJson/Strings/Adapters/RamString.hpp>
#include <ArduinoJson/Strings/Adapters/StringObject.hpp>

#if ARDUINOJSON_ENABLE_PROGMEM
#  include <ArduinoJson/Strings/Adapters/FlashString.hpp>
#endif

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TAdaptedString1, typename TAdaptedString2>
typename enable_if<TAdaptedString1::typeSortKey <= TAdaptedString2::typeSortKey,
                   int>::type
stringCompare(TAdaptedString1 s1, TAdaptedString2 s2) {
  ARDUINOJSON_ASSERT(!s1.isNull());
  ARDUINOJSON_ASSERT(!s2.isNull());
  size_t size1 = s1.size();
  size_t size2 = s2.size();
  size_t n = size1 < size2 ? size1 : size2;
  for (size_t i = 0; i < n; i++) {
    if (s1[i] != s2[i])
      return s1[i] - s2[i];
  }
  if (size1 < size2)
    return -1;
  if (size1 > size2)
    return 1;
  return 0;
}

template <typename TAdaptedString1, typename TAdaptedString2>
typename enable_if<
    (TAdaptedString1::typeSortKey > TAdaptedString2::typeSortKey), int>::type
stringCompare(TAdaptedString1 s1, TAdaptedString2 s2) {
  return -stringCompare(s2, s1);
}

template <typename TAdaptedString1, typename TAdaptedString2>
typename enable_if<TAdaptedString1::typeSortKey <= TAdaptedString2::typeSortKey,
                   bool>::type
stringEquals(TAdaptedString1 s1, TAdaptedString2 s2) {
  ARDUINOJSON_ASSERT(!s1.isNull());
  ARDUINOJSON_ASSERT(!s2.isNull());
  size_t size1 = s1.size();
  size_t size2 = s2.size();
  if (size1 != size2)
    return false;
  for (size_t i = 0; i < size1; i++) {
    if (s1[i] != s2[i])
      return false;
  }
  return true;
}

template <typename TAdaptedString1, typename TAdaptedString2>
typename enable_if<
    (TAdaptedString1::typeSortKey > TAdaptedString2::typeSortKey), bool>::type
stringEquals(TAdaptedString1 s1, TAdaptedString2 s2) {
  return stringEquals(s2, s1);
}

template <typename TAdaptedString>
static void stringGetChars(TAdaptedString s, char* p, size_t n) {
  ARDUINOJSON_ASSERT(s.size() <= n);
  for (size_t i = 0; i < n; i++) {
    p[i] = s[i];
  }
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
