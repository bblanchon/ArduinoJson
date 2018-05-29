// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "./StringCopier.hpp"
#include "./StringMover.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename TJsonBuffer, typename TInput, typename Enable = void>
struct StringStorage {
  typedef StringCopier<TJsonBuffer> type;

  static type create(TJsonBuffer& jb, TInput&) {
    return type(jb);
  }
};

template <typename TJsonBuffer, typename TChar>
struct StringStorage<TJsonBuffer, TChar*,
                     typename enable_if<!is_const<TChar>::value>::type> {
  typedef StringMover<TChar> type;

  static type create(TJsonBuffer&, TChar* input) {
    return type(input);
  }
};

template <typename TJsonBuffer, typename TInput>
typename StringStorage<TJsonBuffer, TInput>::type makeStringStorage(
    TJsonBuffer& jb, TInput& input) {
  return StringStorage<TJsonBuffer, TInput>::create(jb, input);
}

template <typename TJsonBuffer, typename TChar>
typename StringStorage<TJsonBuffer, TChar*>::type makeStringStorage(
    TJsonBuffer& jb, TChar* input) {
  return StringStorage<TJsonBuffer, TChar*>::create(jb, input);
}
}  // namespace Internals
}  // namespace ArduinoJson
