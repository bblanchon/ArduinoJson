// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Deserialization/Filter.hpp>
#include <ArduinoJson/Deserialization/NestingLimit.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TFilter>
struct DeserializationOptions {
  TFilter filter;
  DeserializationOption::NestingLimit nestingLimit;
};

template <typename TFilter>
inline DeserializationOptions<TFilter> makeDeserializationOptions(
    TFilter filter, DeserializationOption::NestingLimit nestingLimit = {}) {
  return {filter, nestingLimit};
}

template <typename TFilter>
inline DeserializationOptions<TFilter> makeDeserializationOptions(
    DeserializationOption::NestingLimit nestingLimit, TFilter filter) {
  return {filter, nestingLimit};
}

inline DeserializationOptions<AllowAllFilter> makeDeserializationOptions(
    DeserializationOption::NestingLimit nestingLimit = {}) {
  return {{}, nestingLimit};
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
