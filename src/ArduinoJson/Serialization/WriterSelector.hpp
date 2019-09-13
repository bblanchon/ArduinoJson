// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TDestination, typename Enable = void>
struct WriterSelector {
  // by default, assume destination implements the Writer concept
  typedef TDestination& writer_type;
};

}  // namespace ARDUINOJSON_NAMESPACE
