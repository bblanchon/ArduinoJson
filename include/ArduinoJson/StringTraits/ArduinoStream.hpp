// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include <Stream.h>

#include "../TypeTraits/EnableIf.hpp"
#include "../TypeTraits/IsBaseOf.hpp"
#include "../TypeTraits/RemoveReference.hpp"

namespace ArduinoJson {
namespace Internals {

struct ArduinoStreamTraits {
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
struct StringTraits<TStream,
                    // match any type that is derived from Stream:
                    typename TypeTraits::EnableIf<TypeTraits::IsBaseOf<
                        Stream, typename TypeTraits::RemoveReference<
                                    TStream>::type>::value>::type>
    : ArduinoStreamTraits {};
}
}
