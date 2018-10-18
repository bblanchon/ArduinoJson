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

namespace ARDUINOJSON_NAMESPACE {

template <typename TStringRef>
class JsonObjectSubscript
    : public JsonVariantBase<JsonObjectSubscript<TStringRef> >,
      public Visitable {
  typedef JsonObjectSubscript<TStringRef> this_type;

 public:
  FORCE_INLINE JsonObjectSubscript(JsonObject object, TStringRef key)
      : _object(object), _key(key) {}

  operator JsonVariantConst() const {
    return get_impl();
  }

  FORCE_INLINE this_type &operator=(const this_type &src) {
    set_impl().set(src);
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
    set_impl().set(src);
    return *this;
  }
  //
  // operator=(TValue);
  // TValue = char*, const char*, const FlashStringHelper*
  template <typename TValue>
  FORCE_INLINE this_type &operator=(TValue *src) {
    set_impl().set(src);
    return *this;
  }

  FORCE_INLINE bool isNull() const {
    return get_impl().isNull();
  }

  template <typename TValue>
  FORCE_INLINE typename JsonVariantAs<TValue>::type as() const {
    return get_impl().template as<TValue>();
  }

  template <typename TValue>
  FORCE_INLINE bool is() const {
    return get_impl().template is<TValue>();
  }

  template <typename TValue>
  FORCE_INLINE typename JsonVariantTo<TValue>::type to() {
    return set_impl().template to<TValue>();
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
    return set_impl().set(value);
  }
  //
  // bool set(TValue);
  // TValue = char*, const char, const FlashStringHelper*
  template <typename TValue>
  FORCE_INLINE bool set(const TValue *value) {
    return set_impl().set(value);
  }

  template <typename Visitor>
  void accept(Visitor &visitor) const {
    return get_impl().accept(visitor);
  }

 private:
  FORCE_INLINE JsonVariant get_impl() const {
    return _object.get(_key);
  }

  FORCE_INLINE JsonVariant set_impl() const {
    return _object.set(_key);
  }

  JsonObject _object;
  TStringRef _key;
};

template <typename TImpl>
template <typename TString>
inline typename enable_if<IsString<TString>::value,
                          JsonObjectSubscript<const TString &> >::type
    JsonVariantSubscripts<TImpl>::operator[](const TString &key) const {
  return impl()->template as<JsonObject>()[key];
}

template <typename TImpl>
template <typename TString>
inline typename enable_if<IsString<TString *>::value,
                          JsonObjectSubscript<TString *> >::type
    JsonVariantSubscripts<TImpl>::operator[](TString *key) const {
  return impl()->template as<JsonObject>()[key];
}

}  // namespace ARDUINOJSON_NAMESPACE

#ifdef _MSC_VER
#pragma warning(pop)
#endif
