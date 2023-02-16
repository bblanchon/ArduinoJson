// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/StringStorage/StringCopier.hpp>
#include <ArduinoJson/StringStorage/StringMover.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TInput>
StringCopier makeStringStorage(TInput&, MemoryPool* pool) {
  ARDUINOJSON_ASSERT(pool != 0);
  return StringCopier(pool);
}

template <typename TChar>
StringMover makeStringStorage(
    TChar* input, MemoryPool*,
    typename enable_if<!is_const<TChar>::value>::type* = 0) {
  return StringMover(reinterpret_cast<char*>(input));
}
ARDUINOJSON_END_PRIVATE_NAMESPACE
