// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Strings/Adapters/RamString.hpp>
#include <ArduinoJson/Strings/StringAdapter.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

namespace string_traits_impl {

// const char* c_str() const
// - String
// - std::string

template <class T, class = void>
struct has_cstr : false_type {};

template <class T>
struct has_cstr<T, enable_if_t<is_same<decltype(declval<const T>().c_str()),
                                       const char*>::value>> : true_type {};

// const char* data() const
// - std::string
// - std::string_view
// - etl::string

template <class T, class = void>
struct has_data : false_type {};

template <class T>
struct has_data<T, enable_if_t<is_same<decltype(declval<const T>().data()),
                                       const char*>::value>> : true_type {};

// unsigned int length() const
// - String

template <class T, class = void>
struct has_length : false_type {};

template <class T>
struct has_length<
    T, enable_if_t<is_unsigned<decltype(declval<const T>().length())>::value>>
    : true_type {};

// size_t size() const
// - std::string
// - std::string_view
// - etl::string

template <class T, class = void>
struct has_size : false_type {};

template <class T>
struct has_size<
    T, enable_if_t<is_same<decltype(declval<const T>().size()), size_t>::value>>
    : true_type {};

}  // namespace string_traits_impl

template <typename T>
struct string_traits {
  enum {
    has_cstr = string_traits_impl::has_cstr<T>::value,
    has_length = string_traits_impl::has_length<T>::value,
    has_data = string_traits_impl::has_data<T>::value,
    has_size = string_traits_impl::has_size<T>::value
  };
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
