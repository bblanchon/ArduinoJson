// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include <istream>
#include "../TypeTraits/EnableIf.hpp"
#include "../TypeTraits/IsBaseOf.hpp"
#include "../TypeTraits/RemoveReference.hpp"

namespace ArduinoJson {
namespace Internals {

struct StdStreamTraits {
  class Iterator {
    std::istream& _stream;

   public:
    Iterator(std::istream& stream) : _stream(stream) {}

    char next() {
      return _stream.eof() ? '\0' : static_cast<char>(_stream.get());
    }

   private:
    Iterator& operator=(const Iterator&);  // Visual Studio C4512
  };
};

template <typename TStream>
struct StringTraits<TStream,
                    // match any type that is derived from std::istream:
                    typename TypeTraits::EnableIf<TypeTraits::IsBaseOf<
                        std::istream, typename TypeTraits::RemoveReference<
                                          TStream>::type>::value>::type>
    : StdStreamTraits {};
}
}
