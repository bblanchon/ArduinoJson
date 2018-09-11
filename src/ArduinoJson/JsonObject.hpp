// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "./JsonObjectIterator.hpp"

// Returns the size (in bytes) of an object with n elements.
// Can be very handy to determine the size of a StaticMemoryPool.
#define JSON_OBJECT_SIZE(NUMBER_OF_ELEMENTS) \
  ((NUMBER_OF_ELEMENTS) * sizeof(ArduinoJson::Internals::Slot))

namespace ArduinoJson {

class JsonObject {
  friend class JsonVariant;

 public:
  typedef JsonObjectIterator iterator;

  FORCE_INLINE JsonObject() : _memoryPool(0), _data(0) {}
  FORCE_INLINE JsonObject(Internals::MemoryPool* buf,
                          Internals::JsonObjectData* object)
      : _memoryPool(buf), _data(object) {}

  FORCE_INLINE iterator begin() const {
    if (!_data) return iterator();
    return iterator(_memoryPool, _data->head);
  }

  void clear() {
    _data->head = 0;
    _data->tail = 0;
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

  bool copyFrom(JsonObject src) {
    bool ok = _data != 0;
    clear();
    for (iterator it = src.begin(); it != src.end(); ++it) {
      ok &= set(it->key(), it->value());
    }
    return ok;
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

  FORCE_INLINE bool operator==(JsonObject rhs) const {
    if (size() != rhs.size()) return false;
    for (iterator it = begin(); it != end(); ++it) {
      if (rhs.get<JsonVariant>(it->key()) != it->value()) return false;
    }
    return true;
  }

  FORCE_INLINE void remove(iterator it) {
    if (!_data) return;
    Internals::Slot* slot = it.internal();
    if (!slot) return;
    if (slot->prev)
      slot->prev->next = slot->next;
    else
      _data->head = slot->next;
    if (slot->next)
      slot->next->prev = slot->prev;
    else
      _data->tail = slot->prev;
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
    return set(key).set(value);
  }
  //
  // bool set(TKey, TValue);
  // TKey = const std::string&, const String&
  // TValue = char*, const char*, const FlashStringHelper*
  template <typename TValue, typename TString>
  FORCE_INLINE bool set(const TString& key, TValue* value) {
    return set(key).set(value);
  }
  //
  // bool set(TKey, const TValue&);
  // TKey = char*, const char*, const FlashStringHelper*
  // TValue = bool, long, int, short, float, double, serialized, JsonVariant,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TString>
  FORCE_INLINE bool set(TString* key, const TValue& value) {
    return set(key).set(value);
  }
  //
  // bool set(TKey, TValue);
  // TKey = char*, const char*, const FlashStringHelper*
  // TValue = char*, const char*, const FlashStringHelper*
  template <typename TValue, typename TString>
  FORCE_INLINE bool set(TString* key, TValue* value) {
    return set(key).set(value);
  }

  template <typename TString>
  FORCE_INLINE JsonVariant set(TString* key) {
    return set_impl<TString*>(key);
  }

  template <typename TString>
  FORCE_INLINE JsonVariant set(const TString& key) {
    return set_impl<const TString&>(key);
  }

  FORCE_INLINE size_t size() const {
    if (!_data) return 0;
    size_t n = 0;
    Internals::Slot* slot = _data->head;
    while (slot) {
      n++;
      slot = slot->next;
    }
    return n;
  }

  FORCE_INLINE bool isNull() const {
    return _data == 0;
  }

  template <typename Visitor>
  FORCE_INLINE void accept(Visitor& visitor) const {
    if (_data)
      visitor.visitObject(*this);
    else
      visitor.visitNull();
  }

 private:
  template <typename TStringRef>
  FORCE_INLINE bool containsKey_impl(TStringRef key) const {
    return findSlot<TStringRef>(key) != 0;
  }

  template <typename TStringRef>
  FORCE_INLINE JsonArray createNestedArray_impl(TStringRef key);

  template <typename TStringRef>
  FORCE_INLINE JsonObject createNestedObject_impl(TStringRef key);

  // Returns the list node that matches the specified key.
  template <typename TStringRef>
  Internals::Slot* findSlot(TStringRef key) {
    if (!_data) return 0;
    Internals::Slot* slot = _data->head;
    while (slot) {
      if (Internals::makeString(key).equals(slot->key)) break;
      slot = slot->next;
    }
    return slot;
  }
  template <typename TStringRef>
  FORCE_INLINE Internals::Slot* findSlot(TStringRef key) const {
    return const_cast<JsonObject*>(this)->findSlot<TStringRef>(key);
  }

  template <typename TStringRef, typename TValue>
  FORCE_INLINE typename Internals::JsonVariantAs<TValue>::type get_impl(
      TStringRef key) const {
    Internals::Slot* slot = findSlot<TStringRef>(key);
    return slot ? JsonVariant(_memoryPool, &slot->value).as<TValue>()
                : TValue();
  }

  template <typename TStringRef, typename TValue>
  FORCE_INLINE bool is_impl(TStringRef key) const {
    Internals::Slot* slot = findSlot<TStringRef>(key);
    return slot ? JsonVariant(_memoryPool, &slot->value).is<TValue>() : false;
  }

  template <typename TStringRef>
  FORCE_INLINE void remove_impl(TStringRef key) {
    if (!_data) return;
    Internals::Slot* slot = findSlot<TStringRef>(key);
    if (!slot) return;
    if (slot->prev)
      slot->prev->next = slot->next;
    else
      _data->head = slot->next;
    if (slot->next)
      slot->next->prev = slot->prev;
    else
      _data->tail = slot->prev;
  }

  template <typename TStringRef>
  FORCE_INLINE JsonVariant set_impl(TStringRef key) {
    if (!_data) return JsonVariant();

    // ignore null key
    if (Internals::makeString(key).is_null()) return JsonVariant();

    // search a matching key
    Internals::Slot* slot = findSlot<TStringRef>(key);
    if (!slot) {
      // add the key
      slot = new (_memoryPool) Internals::Slot();
      if (!slot) return JsonVariant();

      slot->next = 0;

      if (_data->tail) {
        slot->prev = _data->tail;
        _data->tail->next = slot;
        _data->tail = slot;
      } else {
        slot->prev = 0;
        _data->head = slot;
        _data->tail = slot;
      }

      if (!set_key(slot, key)) return JsonVariant();
    }

    return JsonVariant(_memoryPool, &slot->value);
  }

  FORCE_INLINE bool set_key(Internals::Slot* slot, const char* key) {
    slot->key = key;
    return true;
  }

  template <typename T>
  FORCE_INLINE bool set_key(Internals::Slot* slot, const T& key) {
    const char* dup = Internals::makeString(key).save(_memoryPool);
    if (!dup) return false;
    slot->key = dup;
    return true;
  }

  mutable Internals::MemoryPool* _memoryPool;
  mutable Internals::JsonObjectData* _data;
};  // namespace ArduinoJson
}  // namespace ArduinoJson
