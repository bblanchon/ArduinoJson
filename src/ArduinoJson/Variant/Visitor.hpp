// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>
#include <ArduinoJson/Numbers/JsonFloat.hpp>
#include <ArduinoJson/Numbers/JsonInteger.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TResult>
struct Visitor {
  typedef TResult result_type;

  TResult visitArray(const CollectionData&) {
    return TResult();
  }

  TResult visitBoolean(bool) {
    return TResult();
  }

  TResult visitFloat(JsonFloat) {
    return TResult();
  }

  TResult visitSignedInteger(JsonInteger) {
    return TResult();
  }

  TResult visitNull() {
    return TResult();
  }

  TResult visitObject(const CollectionData&) {
    return TResult();
  }

  TResult visitUnsignedInteger(JsonUInt) {
    return TResult();
  }

  TResult visitRawJson(const char*, size_t) {
    return TResult();
  }

  TResult visitString(const char*, size_t) {
    return TResult();
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
