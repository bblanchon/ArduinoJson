// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <string.h>
#include "../Configuration.hpp"
#include "../TypeTraits/EnableIf.hpp"
#include "../TypeTraits/IsBaseOf.hpp"
#include "../TypeTraits/IsChar.hpp"
#include "../TypeTraits/RemoveReference.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename TString, typename Enable = void>
struct StringTraits {};

template <typename TString>
struct StringTraits<const TString, void> : StringTraits<TString> {};

template <typename TString>
struct StringTraits<TString&, void> : StringTraits<TString> {};
}
}

#include "ArduinoStream.hpp"
#include "CharPointer.hpp"
#include "FlashString.hpp"
#include "StdStream.hpp"
#include "StdString.hpp"

namespace ArduinoJson {
namespace TypeTraits {
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
}
