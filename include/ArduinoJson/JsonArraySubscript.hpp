// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "Configuration.hpp"
#include "JsonVariantBase.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4522)
#endif

namespace ArduinoJson {
class JsonArraySubscript : public JsonVariantBase<JsonArraySubscript> {
 public:
  FORCE_INLINE JsonArraySubscript(JsonArray& array, size_t index)
      : _array(array), _index(index) {}

  FORCE_INLINE JsonArraySubscript& operator=(const JsonArraySubscript& src) {
    _array.set(_index, src);
    return *this;
  }

  template <typename T>
  FORCE_INLINE JsonArraySubscript& operator=(const T& src) {
    _array.set(_index, src);
    return *this;
  }

  FORCE_INLINE bool success() const {
    return _index < _array.size();
  }

  template <typename T>
  FORCE_INLINE typename Internals::JsonVariantAs<T>::type as() const {
    return _array.get<T>(_index);
  }

  template <typename T>
  FORCE_INLINE bool is() const {
    return _array.is<T>(_index);
  }

  template <typename TValue>
  FORCE_INLINE void set(const TValue& value) {
    _array.set(_index, value);
  }

 private:
  JsonArray& _array;
  const size_t _index;
};

#if ARDUINOJSON_ENABLE_STD_STREAM
inline std::ostream& operator<<(std::ostream& os,
                                const JsonArraySubscript& source) {
  return source.printTo(os);
}
#endif

inline JsonArraySubscript JsonArray::operator[](size_t index) {
  return JsonArraySubscript(*this, index);
}

template <typename TImplem>
inline const JsonArraySubscript JsonVariantBase<TImplem>::operator[](
    int index) const {
  return asArray()[index];
}

}  // namespace ArduinoJson

#ifdef _MSC_VER
#pragma warning(pop)
#endif
