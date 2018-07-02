// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "./JsonObjectData.hpp"

namespace ArduinoJson {

class JsonObject {
  friend class JsonVariant;

 public:
  typedef Internals::JsonObjectData::iterator iterator;
  typedef Internals::JsonObjectData::const_iterator const_iterator;

  JsonObject() : _data(0) {}
  JsonObject(Internals::JsonObjectData* object) : _data(object) {}
  JsonObject(Internals::JsonBuffer* buf)
      : _data(new (buf) Internals::JsonObjectData(buf)) {}

  iterator begin() {
    if (!_data) return iterator();
    return _data->begin();
  }

  const_iterator begin() const {
    if (!_data) return const_iterator();
    return _data->begin();
  }

  // Tells weither the specified key is present and associated with a value.
  //
  // bool containsKey(TKey);
  // TKey = const std::string&, const String&
  template <typename TString>
  bool containsKey(const TString& key) const {
    return containsKey_impl<const TString&>(key);
  }
  //
  // bool containsKey(TKey);
  // TKey = char*, const char*, char[], const char[], const FlashStringHelper*
  template <typename TString>
  bool containsKey(TString* key) const {
    return containsKey_impl<TString*>(key);
  }

  iterator end() {
    return iterator();
  }

  const_iterator end() const {
    return const_iterator();
  }

  // Creates and adds a JsonArray.
  //
  // JsonArray createNestedArray(TKey);
  // TKey = const std::string&, const String&
  template <typename TString>
  JsonArray createNestedArray(const TString& key);
  // JsonArray createNestedArray(TKey);
  // TKey = char*, const char*, char[], const char[], const FlashStringHelper*
  template <typename TString>
  JsonArray createNestedArray(TString* key);

  // Creates and adds a JsonObject.
  //
  // JsonObject createNestedObject(TKey);
  // TKey = const std::string&, const String&
  template <typename TString>
  JsonObject createNestedObject(const TString& key) {
    if (!_data) return JsonObject();
    return createNestedObject_impl<const TString&>(key);
  }
  //
  // JsonObject createNestedObject(TKey);
  // TKey = char*, const char*, char[], const char[], const FlashStringHelper*
  template <typename TString>
  JsonObject createNestedObject(TString* key) {
    return createNestedObject_impl<TString*>(key);
  }

  // Gets the value associated with the specified key.
  //
  // TValue get<TValue>(TKey) const;
  // TKey = const std::string&, const String&
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TString>
  typename Internals::JsonVariantAs<TValue>::type get(
      const TString& key) const {
    return get_impl<const TString&, TValue>(key);
  }
  //
  // TValue get<TValue>(TKey) const;
  // TKey = char*, const char*, const FlashStringHelper*
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TString>
  typename Internals::JsonVariantAs<TValue>::type get(TString* key) const {
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
  bool is(const TString& key) const {
    return is_impl<const TString&, TValue>(key);
  }
  //
  // bool is<TValue>(TKey) const;
  // TKey = char*, const char*, const FlashStringHelper*
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TString>
  bool is(TString* key) const {
    return is_impl<TString*, TValue>(key);
  }

  // Gets or sets the value associated with the specified key.
  //
  // JsonObjectSubscript operator[](TKey)
  // TKey = const std::string&, const String&
  template <typename TString>
  Internals::JsonObjectSubscript<const TString&> operator[](
      const TString& key) {
    return Internals::JsonObjectSubscript<const TString&>(*this, key);
  }
  //
  // JsonObjectSubscript operator[](TKey)
  // TKey = char*, const char*, char[], const char[N], const FlashStringHelper*
  template <typename TString>
  Internals::JsonObjectSubscript<TString*> operator[](TString* key) {
    return Internals::JsonObjectSubscript<TString*>(*this, key);
  }

  // Gets the value associated with the specified key.
  //
  // const JsonObjectSubscript operator[](TKey) const;
  // TKey = const std::string&, const String&
  template <typename TString>
  const Internals::JsonObjectSubscript<const TString&> operator[](
      const TString& key) const {
    return Internals::JsonObjectSubscript<const TString&>(*this, key);
  }
  //
  // const JsonObjectSubscript operator[](TKey) const;
  // TKey = const char*, const char[N], const FlashStringHelper*
  template <typename TString>
  const Internals::JsonObjectSubscript<TString*> operator[](
      TString* key) const {
    return Internals::JsonObjectSubscript<TString*>(*this, key);
  }

  bool operator==(const JsonObject& rhs) const {
    return _data == rhs._data;
  }

  void remove(iterator it) {
    if (!_data) return;
    _data->remove(it);
  }

  // Removes the specified key and the associated value.
  //
  // void remove(TKey);
  // TKey = const std::string&, const String&
  template <typename TString>
  void remove(const TString& key) {
    remove_impl<const TString&>(key);
  }
  //
  // void remove(TKey);
  // TKey = char*, const char*, char[], const char[], const FlashStringHelper*
  template <typename TString>
  void remove(TString* key) {
    remove_impl<TString*>(key);
  }

  // Sets the specified key with the specified value.
  //
  // bool set(TKey, TValue);
  // TKey = const std::string&, const String&
  // TValue = bool, long, int, short, float, double, RawJson, JsonVariant,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TString>
  bool set(const TString& key, const TValue& value) {
    return set_impl<const TString&, const TValue&>(key, value);
  }
  //
  // bool set(TKey, TValue);
  // TKey = const std::string&, const String&
  // TValue = char*, const char*, const FlashStringHelper*
  template <typename TValue, typename TString>
  bool set(const TString& key, TValue* value) {
    return set_impl<const TString&, TValue*>(key, value);
  }
  //
  // bool set(TKey, const TValue&);
  // TKey = char*, const char*, const FlashStringHelper*
  // TValue = bool, long, int, short, float, double, RawJson, JsonVariant,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TString>
  bool set(TString* key, const TValue& value) {
    return set_impl<TString*, const TValue&>(key, value);
  }
  //
  // bool set(TKey, TValue);
  // TKey = char*, const char*, const FlashStringHelper*
  // TValue = char*, const char*, const FlashStringHelper*
  template <typename TValue, typename TString>
  bool set(TString* key, TValue* value) {
    return set_impl<TString*, TValue*>(key, value);
  }

  size_t size() const {
    if (!_data) return 0;
    return _data->size();
  }

  bool isNull() const {
    return _data == 0;
  }

  template <typename Visitor>
  void visit(Visitor& visitor) const {
    if (_data)
      visitor.acceptObject(*this);
    else
      return visitor.acceptNull();
  }

 private:
  template <typename TStringRef>
  bool containsKey_impl(TStringRef key) const {
    return findKey<TStringRef>(key) != end();
  }

  template <typename TStringRef>
  JsonArray createNestedArray_impl(TStringRef key);

  template <typename TStringRef>
  JsonObject createNestedObject_impl(TStringRef key);

  // Returns the list node that matches the specified key.
  template <typename TStringRef>
  iterator findKey(TStringRef key) {
    iterator it;
    for (it = begin(); it != end(); ++it) {
      if (Internals::StringTraits<TStringRef>::equals(key, it->key)) break;
    }
    return it;
  }
  template <typename TStringRef>
  const_iterator findKey(TStringRef key) const {
    return const_cast<JsonObject*>(this)->findKey<TStringRef>(key);
  }

  template <typename TStringRef, typename TValue>
  typename Internals::JsonVariantAs<TValue>::type get_impl(
      TStringRef key) const {
    const_iterator it = findKey<TStringRef>(key);
    return it != end() ? it->value.as<TValue>()
                       : Internals::JsonVariantDefault<TValue>::get();
  }

  template <typename TStringRef, typename TValue>
  bool is_impl(TStringRef key) const {
    const_iterator it = findKey<TStringRef>(key);
    return it != end() ? it->value.is<TValue>() : false;
  }

  template <typename TStringRef>
  void remove_impl(TStringRef key) {
    if (!_data) return;
    _data->remove(findKey<TStringRef>(key));
  }

  template <typename TStringRef, typename TValueRef>
  bool set_impl(TStringRef key, TValueRef value) {
    if (!_data) return false;

    // ignore null key
    if (Internals::StringTraits<TStringRef>::is_null(key)) return false;

    // search a matching key
    iterator it = findKey<TStringRef>(key);
    if (it == end()) {
      // add the key
      it = _data->add();
      if (it == end()) return false;
      bool key_ok =
          Internals::ValueSaver<TStringRef>::save(_data->_buffer, it->key, key);
      if (!key_ok) return false;
    }

    // save the value
    return Internals::ValueSaver<TValueRef>::save(_data->_buffer, it->value,
                                                  value);
  }

  Internals::JsonObjectData* _data;
};
}  // namespace ArduinoJson
