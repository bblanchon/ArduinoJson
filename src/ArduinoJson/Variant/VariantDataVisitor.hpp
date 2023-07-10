// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ArrayData.hpp>
#include <ArduinoJson/Numbers/JsonFloat.hpp>
#include <ArduinoJson/Numbers/JsonInteger.hpp>
#include <ArduinoJson/Object/ObjectData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TResult>
struct VariantDataVisitor {
  typedef TResult result_type;

  TResult visitArray(const ArrayData&) {
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

  TResult visitObject(const ObjectData&) {
    return TResult();
  }

  TResult visitUnsignedInteger(JsonUInt) {
    return TResult();
  }

  TResult visitRawString(const char*, size_t) {
    return TResult();
  }

  TResult visitString(const char*, size_t) {
    return TResult();
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
