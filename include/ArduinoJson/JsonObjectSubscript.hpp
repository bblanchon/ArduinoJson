// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonVariantBase.hpp"

namespace ArduinoJson {
class JsonObjectSubscript : public JsonVariantBase<JsonObjectSubscript> {
 public:
  JSON_FORCE_INLINE JsonObjectSubscript(JsonObject& object, const char* key)
      : _object(object), _key(key) {}

  JSON_FORCE_INLINE JsonObjectSubscript& operator=(const JsonVariant& value) {
    _object.set(_key, value);
    return *this;
  }

  JSON_FORCE_INLINE bool success() const { return _object.containsKey(_key); }

  JSON_FORCE_INLINE operator JsonVariant() const { return _object.get(_key); }

  template <typename T>
  JSON_FORCE_INLINE T as() const {
    return _object.get<T>(_key);
  }

  template <typename T>
  JSON_FORCE_INLINE T is() const {
    return _object.is<T>(_key);
  }

 private:
  JsonObject& _object;
  const char* _key;
};
}
