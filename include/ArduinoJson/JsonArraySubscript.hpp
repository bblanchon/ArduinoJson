// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonVariantBase.hpp"

namespace ArduinoJson {
class JsonArraySubscript : public JsonVariantBase<JsonArraySubscript> {
 public:
  JSON_FORCE_INLINE JsonArraySubscript(JsonArray& array, size_t index)
      : _array(array), _index(index) {}

  JSON_FORCE_INLINE JsonArraySubscript& operator=(const JsonVariant& value) {
    _array.set(_index, value);
    return *this;
  }

  JSON_FORCE_INLINE bool success() const { return _index < _array.size(); }

  JSON_FORCE_INLINE operator JsonVariant() const { return _array.get(_index); }

  template <typename T>
  JSON_FORCE_INLINE T as() const {
    return _array.get<T>(_index);
  }

  template <typename T>
  JSON_FORCE_INLINE T is() const {
    return _array.is<T>(_index);
  }

 private:
  JsonArray& _array;
  const size_t _index;
};
}
