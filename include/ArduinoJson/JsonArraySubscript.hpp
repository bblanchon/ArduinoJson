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
  FORCE_INLINE JsonArraySubscript(JsonArray& array, size_t index)
      : _array(array), _index(index) {}

  FORCE_INLINE JsonArraySubscript& operator=(const JsonVariant& value) {
    _array.set(_index, value);
    return *this;
  }

  FORCE_INLINE JsonArraySubscript& operator=(const JsonArraySubscript& other) {
    // to prevent Visual Studio warning C4512: assignment operator could not be
    // generated
    _array.set(_index, other._array.get(other._index));
    return *this;
  }

  FORCE_INLINE bool success() const { return _index < _array.size(); }

  FORCE_INLINE operator JsonVariant() const { return _array.get(_index); }

  template <typename T>
  FORCE_INLINE T as() const {
    return _array.get<T>(_index);
  }

  template <typename T>
  FORCE_INLINE T is() const {
    return _array.is<T>(_index);
  }

  void writeTo(Internals::JsonWriter &writer) const {
    _array.get(_index).writeTo(writer);
  }

 private:
  JsonArray& _array;
  const size_t _index;
};

#ifdef ARDUINOJSON_ENABLE_STD_STREAM
inline std::ostream& operator<<(std::ostream& os, const JsonArraySubscript& source) {
  return source.printTo(os);
}
#endif

} // namespace ArduinoJson
