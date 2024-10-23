// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <bool Condition, class TrueType, class FalseType>
struct conditional {
  using type = TrueType;
};

template <class TrueType, class FalseType>
struct conditional<false, TrueType, FalseType> {
  using type = FalseType;
};

template <bool Condition, class TrueType, class FalseType>
using conditional_t =
    typename conditional<Condition, TrueType, FalseType>::type;

ARDUINOJSON_END_PRIVATE_NAMESPACE
