// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../TypeTraits/EnableIf.hpp"
#include "../TypeTraits/IsBaseOf.hpp"
#include "../TypeTraits/RemoveReference.hpp"

namespace ArduinoJson {
namespace Internals {

struct StdStreamFuncs {
  class Iterator {
    Stream& _stream;

   public:
    Iterator(Stream& stream) : _stream(stream) {}

    char next() {
      int n = _stream.read();
      return n >= 0 ? static_cast<char>(n) : '\0';
    }
  };
};

template <typename TStream>
struct StringFuncs<TStream,
                   // match any type that is derived from std::istream:
                   typename TypeTraits::EnableIf<TypeTraits::IsBaseOf<
                       Stream, typename TypeTraits::RemoveReference<
                                   TStream>::type>::value>::type>
    : StdStreamFuncs {};
}
}
