// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>
#include <ArduinoJson/Numbers/JsonFloat.hpp>
#include <ArduinoJson/Numbers/JsonInteger.hpp>

namespace ARDUINOJSON_NAMESPACE {

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

}  // namespace ARDUINOJSON_NAMESPACE
