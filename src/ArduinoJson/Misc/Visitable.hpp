// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/type_traits.hpp>

namespace ARDUINOJSON_NAMESPACE {

struct Visitable {
  // template<Visitor>
  // void accept(Visitor&) const;
};

template <typename T>
struct IsVisitable : is_base_of<Visitable, T> {};

template <typename T>
struct IsVisitable<T &> : IsVisitable<T> {};
}  // namespace ARDUINOJSON_NAMESPACE
