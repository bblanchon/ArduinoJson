// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/utility.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TString, typename Enable = void>
struct StringAdapter;

template <typename TString, typename Enable = void>
struct SizedStringAdapter;

template <typename TString>
using StringAdapterFor = StringAdapter<decay_t<TString>>;

template <typename T>
using AdaptedString = typename StringAdapterFor<T>::AdaptedString;

template <typename TString>
AdaptedString<TString> adaptString(TString&& s) {
  return StringAdapterFor<TString>::adapt(detail::forward<TString>(s));
}

template <typename TChar>
AdaptedString<TChar*> adaptString(TChar* p) {
  return StringAdapter<TChar*>::adapt(p);
}

template <typename TChar>
AdaptedString<TChar*> adaptString(TChar* p, size_t n) {
  return SizedStringAdapter<TChar*>::adapt(p, n);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
