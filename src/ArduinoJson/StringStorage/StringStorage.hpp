// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/StringStorage/StringCopier.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TInput>
StringCopier makeStringStorage(TInput&, MemoryPool* pool) {
  ARDUINOJSON_ASSERT(pool != 0);
  return StringCopier(pool);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
