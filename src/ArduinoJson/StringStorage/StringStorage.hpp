// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/StringStorage/StringCopier.hpp>
#include <ArduinoJson/StringStorage/StringMover.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TInput, typename Enable = void>
struct StringStorage {
  typedef StringCopier type;

  static type create(TInput&) {
    return type();
  }
};

template <typename TChar>
struct StringStorage<TChar*,
                     typename enable_if<!is_const<TChar>::value>::type> {
  typedef StringMover type;

  static type create(TChar* input) {
    return type(reinterpret_cast<char*>(input));
  }
};

template <typename TInput>
typename StringStorage<TInput>::type makeStringStorage(TInput& input) {
  return StringStorage<TInput>::create(input);
}

template <typename TChar>
typename StringStorage<TChar*>::type makeStringStorage(TChar* input) {
  return StringStorage<TChar*>::create(input);
}
}  // namespace ARDUINOJSON_NAMESPACE
