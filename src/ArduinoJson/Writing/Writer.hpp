// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "./JsonBufferWriter.hpp"
#include "./StringWriter.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename TJsonBuffer, typename TInput, typename Enable = void>
struct Writer {
  typedef JsonBufferWriter<TJsonBuffer> type;

  static type create(TJsonBuffer& jb, TInput&) {
    return type(jb);
  }
};

template <typename TJsonBuffer, typename TChar>
struct Writer<TJsonBuffer, TChar*,
              typename enable_if<!is_const<TChar>::value>::type> {
  typedef StringWriter<TChar> type;

  static type create(TJsonBuffer&, TChar* input) {
    return type(input);
  }
};

template <typename TJsonBuffer, typename TInput>
typename Writer<TJsonBuffer, TInput>::type makeWriter(TJsonBuffer& jb,
                                                      TInput& input) {
  return Writer<TJsonBuffer, TInput>::create(jb, input);
}

template <typename TJsonBuffer, typename TChar>
typename Writer<TJsonBuffer, TChar*>::type makeWriter(TJsonBuffer& jb,
                                                      TChar* input) {
  return Writer<TJsonBuffer, TChar*>::create(jb, input);
}
}  // namespace Internals
}  // namespace ArduinoJson
