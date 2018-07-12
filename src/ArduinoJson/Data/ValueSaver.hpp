// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../JsonVariant.hpp"
#include "../Memory/JsonBuffer.hpp"
#include "../Polyfills/type_traits.hpp"
#include "../Strings/StringTypes.hpp"

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

// We duplicate all strings except const char*
template <typename TString>
struct ValueSaver<
    TString, typename enable_if<IsString<TString>::value &&
                                !is_same<const char*, TString>::value>::type> {
  template <typename Destination>
  static bool save(JsonBuffer* buffer, Destination& dest, TString source) {
    const char* dup = makeString(source).save(buffer);
    if (!dup) return false;
    dest = dup;
    return true;
  }
};

// We duplicate all SerializedValue<T> except SerializedValue<const char*>
template <typename TString>
struct ValueSaver<
    const SerializedValue<TString>&,
    typename enable_if<!is_same<const char*, TString>::value>::type> {
  template <typename Destination>
  static bool save(JsonBuffer* buffer, Destination& dest,
                   const SerializedValue<TString>& source) {
    const char* dup = makeString(source.data(), source.size()).save(buffer);
    if (!dup) return false;
    dest = SerializedValue<const char*>(dup, source.size());
    return true;
  }
};
}  // namespace Internals
}  // namespace ArduinoJson
