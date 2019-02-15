// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "../Polyfills/type_traits.hpp"

namespace ARDUINOJSON_NAMESPACE {

struct Visitable {
  // template<Visitor>
  // void accept(Visitor&) const;
};

template <typename T>
struct IsVisitable : is_base_of<Visitable, T> {};

template <typename T>
struct IsVisitable<T&> : IsVisitable<T> {};
}  // namespace ARDUINOJSON_NAMESPACE
