// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "./StringCopier.hpp"
#include "./StringMover.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename TMemoryPool, typename TInput, typename Enable = void>
struct StringStorage {
  typedef StringCopier<TMemoryPool> type;

  static type create(TMemoryPool& jb, TInput&) {
    return type(jb);
  }
};

template <typename TMemoryPool, typename TChar>
struct StringStorage<TMemoryPool, TChar*,
                     typename enable_if<!is_const<TChar>::value>::type> {
  typedef StringMover<TChar> type;

  static type create(TMemoryPool&, TChar* input) {
    return type(input);
  }
};

template <typename TMemoryPool, typename TInput>
typename StringStorage<TMemoryPool, TInput>::type makeStringStorage(
    TMemoryPool& jb, TInput& input) {
  return StringStorage<TMemoryPool, TInput>::create(jb, input);
}

template <typename TMemoryPool, typename TChar>
typename StringStorage<TMemoryPool, TChar*>::type makeStringStorage(
    TMemoryPool& jb, TChar* input) {
  return StringStorage<TMemoryPool, TChar*>::create(jb, input);
}
}  // namespace Internals
}  // namespace ArduinoJson
