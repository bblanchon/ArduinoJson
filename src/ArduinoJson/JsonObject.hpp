// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "./JsonObjectData.hpp"
#include "./JsonObjectIterator.hpp"

namespace ArduinoJson {

class JsonObject {
  friend class JsonVariant;
  typedef Internals::JsonObjectData::iterator internal_iterator;

 public:
  typedef JsonObjectIterator iterator;

  FORCE_INLINE JsonObject() : _buffer(0), _data(0) {}
  FORCE_INLINE JsonObject(Internals::JsonBuffer* buf,
                          Internals::JsonObjectData* object)
      : _buffer(buf), _data(object) {}
  FORCE_INLINE explicit JsonObject(Internals::JsonBuffer* buf)
      : _buffer(buf), _data(new (buf) Internals::JsonObjectData()) {}

  FORCE_INLINE iterator begin() const {
    if (!_data) return iterator();
    return iterator(_buffer, _data->begin());
  }

  // Tells weither the specified key is present and associated with a value.
  //
  // bool containsKey(TKey);
  // TKey = const std::string&, const String&
  template <typename TString>
  FORCE_INLINE bool containsKey(const TString& key) const {
    return containsKey_impl<const TString&>(key);
  }
  //
  // bool containsKey(TKey);
  // TKey = char*, const char*, char[], const char[], const FlashStringHelper*
  template <typename TString>
  FORCE_INLINE bool containsKey(TString* key) const {
    return containsKey_impl<TString*>(key);
  }

  FORCE_INLINE iterator end() const {
    return iterator();
  }

  // Creates and adds a JsonArray.
  //
  // JsonArray createNestedArray(TKey);
  // TKey = const std::string&, const String&
  template <typename TString>
  FORCE_INLINE JsonArray createNestedArray(const TString& key);
  // JsonArray createNestedArray(TKey);
  // TKey = char*, const char*, char[], const char[], const FlashStringHelper*
  template <typename TString>
  FORCE_INLINE JsonArray createNestedArray(TString* key);

  // Creates and adds a JsonObject.
  //
  // JsonObject createNestedObject(TKey);
  // TKey = const std::string&, const String&
  template <typename TString>
  FORCE_INLINE JsonObject createNestedObject(const TString& key) {
    if (!_data) return JsonObject();
    return createNestedObject_impl<const TString&>(key);
  }
  //
  // JsonObject createNestedObject(TKey);
  // TKey = char*, const char*, char[], const char[], const FlashStringHelper*
  template <typename TString>
  FORCE_INLINE JsonObject createNestedObject(TString* key) {
    return createNestedObject_impl<TString*>(key);
  }

  // Gets the value associated with the specified key.
  //
  // TValue get<TValue>(TKey) const;
  // TKey = const std::string&, const String&
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TString>
  FORCE_INLINE typename Internals::JsonVariantAs<TValue>::type get(
      const TString& key) const {
    return get_impl<const TString&, TValue>(key);
  }
  //
  // TValue get<TValue>(TKey) const;
  // TKey = char*, const char*, const FlashStringHelper*
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TString>
  FORCE_INLINE typename Internals::JsonVariantAs<TValue>::type get(
      TString* key) const {
    return get_impl<TString*, TValue>(key);
  }

  // Checks the type of the value associated with the specified key.
  //
  //
  // bool is<TValue>(TKey) const;
  // TKey = const std::string&, const String&
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TString>
  FORCE_INLINE bool is(const TString& key) const {
    return is_impl<const TString&, TValue>(key);
  }
  //
  // bool is<TValue>(TKey) const;
  // TKey = char*, const char*, const FlashStringHelper*
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TString>
  FORCE_INLINE bool is(TString* key) const {
    return is_impl<TString*, TValue>(key);
  }

  // Gets or sets the value associated with the specified key.
  //
  // JsonObjectSubscript operator[](TKey)
  // TKey = const std::string&, const String&
  template <typename TString>
  FORCE_INLINE Internals::JsonObjectSubscript<const TString&> operator[](
      const TString& key) {
    return Internals::JsonObjectSubscript<const TString&>(*this, key);
  }
  //
  // JsonObjectSubscript operator[](TKey)
  // TKey = char*, const char*, char[], const char[N], const FlashStringHelper*
  template <typename TString>
  FORCE_INLINE Internals::JsonObjectSubscript<TString*> operator[](
      TString* key) {
    return Internals::JsonObjectSubscript<TString*>(*this, key);
  }

  // Gets the value associated with the specified key.
  //
  // const JsonObjectSubscript operator[](TKey) const;
  // TKey = const std::string&, const String&
  template <typename TString>
  FORCE_INLINE const Internals::JsonObjectSubscript<const TString&> operator[](
      const TString& key) const {
    return Internals::JsonObjectSubscript<const TString&>(*this, key);
  }
  //
  // const JsonObjectSubscript operator[](TKey) const;
  // TKey = const char*, const char[N], const FlashStringHelper*
  template <typename TString>
  FORCE_INLINE const Internals::JsonObjectSubscript<TString*> operator[](
      TString* key) const {
    return Internals::JsonObjectSubscript<TString*>(*this, key);
  }

  FORCE_INLINE bool operator==(const JsonObject& rhs) const {
    return _data == rhs._data;
  }

  FORCE_INLINE void remove(iterator it) {
    if (!_data) return;
    _data->remove(it.internal());
  }

  // Removes the specified key and the associated value.
  //
  // void remove(TKey);
  // TKey = const std::string&, const String&
  template <typename TString>
  FORCE_INLINE void remove(const TString& key) {
    remove_impl<const TString&>(key);
  }
  //
  // void remove(TKey);
  // TKey = char*, const char*, char[], const char[], const FlashStringHelper*
  template <typename TString>
  FORCE_INLINE void remove(TString* key) {
    remove_impl<TString*>(key);
  }

  // Sets the specified key with the specified value.
  //
  // bool set(TKey, TValue);
  // TKey = const std::string&, const String&
  // TValue = bool, long, int, short, float, double, serialized, JsonVariant,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TString>
  FORCE_INLINE bool set(const TString& key, const TValue& value) {
    return set_impl<const TString&, const TValue&>(key, value);
  }
  //
  // bool set(TKey, TValue);
  // TKey = const std::string&, const String&
  // TValue = char*, const char*, const FlashStringHelper*
  template <typename TValue, typename TString>
  FORCE_INLINE bool set(const TString& key, TValue* value) {
    return set_impl<const TString&, TValue*>(key, value);
  }
  //
  // bool set(TKey, const TValue&);
  // TKey = char*, const char*, const FlashStringHelper*
  // TValue = bool, long, int, short, float, double, serialized, JsonVariant,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TString>
  FORCE_INLINE bool set(TString* key, const TValue& value) {
    return set_impl<TString*, const TValue&>(key, value);
  }
  //
  // bool set(TKey, TValue);
  // TKey = char*, const char*, const FlashStringHelper*
  // TValue = char*, const char*, const FlashStringHelper*
  template <typename TValue, typename TString>
  FORCE_INLINE bool set(TString* key, TValue* value) {
    return set_impl<TString*, TValue*>(key, value);
  }

  FORCE_INLINE size_t size() const {
    if (!_data) return 0;
    return _data->size();
  }

  FORCE_INLINE bool isNull() const {
    return _data == 0;
  }

  template <typename Visitor>
  FORCE_INLINE void visit(Visitor& visitor) const {
    if (_data)
      visitor.acceptObject(*_data);
    else
      visitor.acceptNull();
  }

 private:
  template <typename TStringRef>
  FORCE_INLINE bool containsKey_impl(TStringRef key) const {
    return findKey<TStringRef>(key) != _data->end();
  }

  template <typename TStringRef>
  FORCE_INLINE JsonArray createNestedArray_impl(TStringRef key);

  template <typename TStringRef>
  FORCE_INLINE JsonObject createNestedObject_impl(TStringRef key);

  // Returns the list node that matches the specified key.
  template <typename TStringRef>
  internal_iterator findKey(TStringRef key) {
    if (!_data) return internal_iterator();
    internal_iterator it;
    for (it = _data->begin(); it != _data->end(); ++it) {
      if (Internals::makeString(key).equals(it->key)) break;
    }
    return it;
  }
  template <typename TStringRef>
  FORCE_INLINE internal_iterator findKey(TStringRef key) const {
    return const_cast<JsonObject*>(this)->findKey<TStringRef>(key);
  }

  template <typename TStringRef, typename TValue>
  FORCE_INLINE typename Internals::JsonVariantAs<TValue>::type get_impl(
      TStringRef key) const {
    internal_iterator it = findKey<TStringRef>(key);
    return it != _data->end() ? JsonVariant(_buffer, &it->value).as<TValue>()
                              : TValue();
  }

  template <typename TStringRef, typename TValue>
  FORCE_INLINE bool is_impl(TStringRef key) const {
    internal_iterator it = findKey<TStringRef>(key);
    return it != _data->end() ? JsonVariant(_buffer, &it->value).is<TValue>()
                              : false;
  }

  template <typename TStringRef>
  FORCE_INLINE void remove_impl(TStringRef key) {
    if (!_data) return;
    _data->remove(findKey<TStringRef>(key));
  }

  template <typename TStringRef, typename TValueRef>
  FORCE_INLINE bool set_impl(TStringRef key, TValueRef value) {
    if (!_data) return false;

    // ignore null key
    if (Internals::makeString(key).is_null()) return false;

    // search a matching key
    internal_iterator it = findKey<TStringRef>(key);
    if (it == _data->end()) {
      // add the key
      // TODO: use JsonPairData directly, we don't need an iterator
      it = _data->add(_buffer);
      if (it == _data->end()) return false;
      if (!set_key(it, key)) return false;
    }

    // save the value
    return JsonVariant(_buffer, &it->value).set(value);
  }

  FORCE_INLINE bool set_key(internal_iterator& it, const char* key) {
    it->key = key;
    return true;
  }

  template <typename T>
  FORCE_INLINE bool set_key(internal_iterator& it, const T& key) {
    const char* dup = Internals::makeString(key).save(_buffer);
    if (!dup) return false;
    it->key = dup;
    return true;
  }

  mutable Internals::JsonBuffer* _buffer;
  mutable Internals::JsonObjectData* _data;
};
}  // namespace ArduinoJson
