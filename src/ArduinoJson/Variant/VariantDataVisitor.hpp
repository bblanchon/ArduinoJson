// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Numbers/JsonFloat.hpp>
#include <ArduinoJson/Numbers/JsonInteger.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TResult>
struct VariantDataVisitor {
  using result_type = TResult;

  template <typename T>
  TResult visit(const T&) {
    return TResult();
  }

  TResult visitArray(VariantData*) {
    return TResult();
  }

  TResult visitObject(VariantData*) {
    return TResult();
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
