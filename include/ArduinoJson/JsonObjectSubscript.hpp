// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "Configuration.hpp"
#include "JsonVariantBase.hpp"
#include "TypeTraits/EnableIf.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4522)
#endif

namespace ArduinoJson {

template <typename TKey>
class JsonObjectSubscript : public JsonVariantBase<JsonObjectSubscript<TKey> > {
 public:
  FORCE_INLINE JsonObjectSubscript(JsonObject& object, TKey key)
      : _object(object), _key(key) {}

  JsonObjectSubscript<TKey>& operator=(const JsonObjectSubscript<TKey>& src) {
    _object.set<const JsonVariant&>(_key, src);
    return *this;
  }

  template <typename T>
  typename TypeTraits::EnableIf<JsonObject::CanSet<T&>::value,
                                JsonObjectSubscript<TKey> >::type&
  operator=(const T& src) {
    _object.set<T&>(_key, const_cast<T&>(src));
    return *this;
  }

  template <typename T>
  typename TypeTraits::EnableIf<JsonObject::CanSet<T>::value,
                                JsonObjectSubscript<TKey> >::type&
  operator=(T src) {
    _object.set<T>(_key, src);
    return *this;
  }

  FORCE_INLINE bool success() const { return _object.containsKey(_key); }

  FORCE_INLINE operator JsonVariant() const { return _object.get(_key); }

  template <typename TValue>
  FORCE_INLINE TValue as() const {
    return _object.get<TValue>(_key);
  }

  template <typename TValue>
  FORCE_INLINE bool is() const {
    return _object.is<TValue>(_key);
  }

  template <typename TValue>
  FORCE_INLINE bool set(TValue value) {
    return _object.set<TValue>(_key, value);
  }

  template <typename TValue>
  FORCE_INLINE bool set(TValue value, uint8_t decimals) {
    return _object.set(_key, value, decimals);
  }

  FORCE_INLINE JsonVariant get() { return _object.get(_key); }

  void writeTo(Internals::JsonWriter& writer) const {
    _object.get(_key).writeTo(writer);
  }

 private:
  JsonObject& _object;
  TKey _key;
};

#if ARDUINOJSON_ENABLE_STD_STREAM
inline std::ostream& operator<<(
    std::ostream& os, const JsonObjectSubscript<const String&>& source) {
  return source.printTo(os);
}

inline std::ostream& operator<<(
    std::ostream& os, const JsonObjectSubscript<const char*>& source) {
  return source.printTo(os);
}
#endif

}  // namespace ArduinoJson

#ifdef _MSC_VER
#pragma warning(pop)
#endif
