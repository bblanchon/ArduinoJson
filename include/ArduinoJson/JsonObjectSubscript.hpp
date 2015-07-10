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
  FORCE_INLINE JsonObjectSubscript(JsonObject& object, JsonObjectKey key)
      : _object(object), _key(key) {}

  FORCE_INLINE JsonObjectSubscript& operator=(const JsonVariant& value) {
    _object.set(_key, value);
    return *this;
  }

  FORCE_INLINE JsonObjectSubscript& operator=(
      const JsonObjectSubscript& other) {
    // to prevent Visual Studio warning C4512: assignment operator could not be
    // generated
    _object.set(_key, other._object.get(other._key));
    return *this;
  }

  FORCE_INLINE bool success() const { return _object.containsKey(_key); }

  FORCE_INLINE operator JsonVariant() const { return _object.get(_key); }

  template <typename T>
  FORCE_INLINE T as() const {
    return _object.get<T>(_key);
  }

  template <typename T>
  FORCE_INLINE T is() const {
    return _object.is<T>(_key);
  }

  void writeTo(Internals::JsonWriter &writer) const {
    _object.get(_key).writeTo(writer);
  }

 private:
  JsonObject& _object;
  JsonObjectKey _key;
};

#ifdef ARDUINOJSON_ENABLE_STD_STREAM
inline std::ostream& operator<<(std::ostream& os, const JsonObjectSubscript& source) {
  return source.printTo(os);
}
#endif
}
