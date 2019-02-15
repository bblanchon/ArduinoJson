// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

namespace ARDUINOJSON_NAMESPACE {

template <bool Condition, class TrueType, class FalseType>
struct conditional {
  typedef TrueType type;
};

template <class TrueType, class FalseType>
struct conditional<false, TrueType, FalseType> {
  typedef FalseType type;
};
}  // namespace ARDUINOJSON_NAMESPACE
