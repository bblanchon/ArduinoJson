// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../JsonBuffer.hpp"
#include "../JsonVariant.hpp"
#include "../StringTraits/StringTraits.hpp"
#include "../TypeTraits/EnableIf.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename Source, typename Enable = void>
struct ValueSaver {
  template <typename Destination>
  static bool save(JsonBuffer*, Destination& destination, Source source) {
    destination = source;
    return true;
  }
};

template <typename Source>
struct ValueSaver<Source, typename TypeTraits::EnableIf<
                              TypeTraits::IsString<Source>::value>::type> {
  template <typename Destination>
  static bool save(JsonBuffer* buffer, Destination& destination,
                   Source source) {
    return StringTraits<Source>::save(source, destination, buffer);
  }
};
}
}
