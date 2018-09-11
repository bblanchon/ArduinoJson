// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Configuration.hpp"
#include "JsonVariantBase.hpp"
#include "Polyfills/type_traits.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4522)
#endif

namespace ArduinoJson {
namespace Internals {

template <typename TStringRef>
class JsonObjectSubscript
    : public JsonVariantBase<JsonObjectSubscript<TStringRef> > {
  typedef JsonObjectSubscript<TStringRef> this_type;

 public:
  FORCE_INLINE JsonObjectSubscript(JsonObject object, TStringRef key)
      : _object(object), _key(key) {}

  FORCE_INLINE this_type &operator=(const this_type &src) {
    _object.set(_key, src);
    return *this;
  }

  // Set the specified value
  //
  // operator=(const TValue&);
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue>
  FORCE_INLINE typename enable_if<!is_array<TValue>::value, this_type &>::type
  operator=(const TValue &src) {
    _object.set(_key, src);
    return *this;
  }
  //
  // operator=(TValue);
  // TValue = char*, const char*, const FlashStringHelper*
  template <typename TValue>
  FORCE_INLINE this_type &operator=(TValue *src) {
    _object.set(_key, src);
    return *this;
  }

  FORCE_INLINE bool isNull() const {
    return !_object.containsKey(_key);
  }

  template <typename TValue>
  FORCE_INLINE typename JsonVariantAs<TValue>::type as() const {
    return _object.get<TValue>(_key);
  }

  template <typename TValue>
  FORCE_INLINE bool is() const {
    return _object.is<TValue>(_key);
  }

  template <typename TValue>
  FORCE_INLINE typename JsonVariantTo<TValue>::type to() {
    return _object.set(_key).template to<TValue>();
  }

  // Sets the specified value.
  //
  // bool set(const TValue&);
  // TValue = bool, char, long, int, short, float, double, serialized,
  // JsonVariant,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue>
  FORCE_INLINE typename enable_if<!is_array<TValue>::value, bool>::type set(
      const TValue &value) {
    return _object.set(_key, value);
  }
  //
  // bool set(TValue);
  // TValue = char*, const char, const FlashStringHelper*
  template <typename TValue>
  FORCE_INLINE bool set(const TValue *value) {
    return _object.set(_key, value);
  }

  template <typename Visitor>
  void accept(Visitor &visitor) const {
    return _object.get<JsonVariant>(_key).accept(visitor);
  }

 private:
  JsonObject _object;
  TStringRef _key;
};

template <typename TImpl>
template <typename TString>
inline typename enable_if<IsString<TString>::value,
                          const JsonObjectSubscript<const TString &> >::type
    JsonVariantSubscripts<TImpl>::operator[](const TString &key) const {
  return impl()->template as<JsonObject>()[key];
}

template <typename TImpl>
template <typename TString>
inline typename enable_if<IsString<TString>::value,
                          JsonObjectSubscript<const TString &> >::type
    JsonVariantSubscripts<TImpl>::operator[](const TString &key) {
  return impl()->template as<JsonObject>()[key];
}

template <typename TImpl>
template <typename TString>
inline typename enable_if<IsString<TString *>::value,
                          JsonObjectSubscript<TString *> >::type
    JsonVariantSubscripts<TImpl>::operator[](TString *key) {
  return impl()->template as<JsonObject>()[key];
}

template <typename TImpl>
template <typename TString>
inline typename enable_if<IsString<TString *>::value,
                          const JsonObjectSubscript<TString *> >::type
    JsonVariantSubscripts<TImpl>::operator[](TString *key) const {
  return impl()->template as<JsonObject>()[key];
}

}  // namespace Internals
}  // namespace ArduinoJson

#ifdef _MSC_VER
#pragma warning(pop)
#endif
