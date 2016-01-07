// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "JsonSubscriptBase.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4522)
#endif

namespace ArduinoJson {
class JsonArraySubscript : public JsonSubscriptBase<JsonArraySubscript> {
 public:
  FORCE_INLINE JsonArraySubscript(JsonArray& array, size_t index)
      : _array(array), _index(index) {}

  using JsonSubscriptBase<JsonArraySubscript>::operator=;

  JsonArraySubscript& operator=(const JsonArraySubscript& src) {
    return assign<const JsonVariant&>(src);
  }

  template <typename T>
  JsonArraySubscript& operator=(const T& src) {
    return assign<const JsonVariant&>(src);
  }

  FORCE_INLINE bool success() const { return _index < _array.size(); }

  FORCE_INLINE operator JsonVariant() const { return _array.get(_index); }

  template <typename T>
  FORCE_INLINE T as() const {
    return _array.get<T>(_index);
  }

  template <typename T>
  FORCE_INLINE bool is() const {
    return _array.is<T>(_index);
  }

  void writeTo(Internals::JsonWriter& writer) const {
    _array.get(_index).writeTo(writer);
  }

  template <typename TValue>
  void set(TValue value) {
    _array.set(_index, value);
  }

 private:
  JsonArray& _array;
  const size_t _index;
};

#ifdef ARDUINOJSON_ENABLE_STD_STREAM
inline std::ostream& operator<<(std::ostream& os,
                                const JsonArraySubscript& source) {
  return source.printTo(os);
}
#endif

}  // namespace ArduinoJson

#ifdef _MSC_VER
#pragma warning(pop)
#endif
