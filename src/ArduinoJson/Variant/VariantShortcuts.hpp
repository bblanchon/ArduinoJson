// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Array/ArrayShortcuts.hpp>
#include <ArduinoJson/Object/ObjectShortcuts.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TVariant>
class VariantShortcuts : public ObjectShortcuts<TVariant>,
                         public ArrayShortcuts<TVariant> {
 public:
  using ArrayShortcuts<TVariant>::createNestedArray;
  using ArrayShortcuts<TVariant>::createNestedObject;
  using ArrayShortcuts<TVariant>::operator[];
  using ObjectShortcuts<TVariant>::createNestedArray;
  using ObjectShortcuts<TVariant>::createNestedObject;
  using ObjectShortcuts<TVariant>::operator[];
};
}  // namespace ARDUINOJSON_NAMESPACE
