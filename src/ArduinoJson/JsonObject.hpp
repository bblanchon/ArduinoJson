// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/ObjectFunctions.hpp"
#include "JsonObjectIterator.hpp"

// Returns the size (in bytes) of an object with n elements.
// Can be very handy to determine the size of a StaticMemoryPool.
#define JSON_OBJECT_SIZE(NUMBER_OF_ELEMENTS) \
  ((NUMBER_OF_ELEMENTS) * sizeof(ARDUINOJSON_NAMESPACE::Slot))

namespace ARDUINOJSON_NAMESPACE {

template <typename TData>
class JsonObjectProxy {
 public:
  // Tells weither the specified key is present and associated with a value.
  //
  // bool containsKey(TKey);
  // TKey = const std::string&, const String&
  template <typename TKey>
  FORCE_INLINE bool containsKey(const TKey& key) const {
    return objectContainsKey(_data, makeString(key));
  }
  //
  // bool containsKey(TKey);
  // TKey = char*, const char*, char[], const char[], const FlashStringHelper*
  template <typename TKey>
  FORCE_INLINE bool containsKey(TKey* key) const {
    return objectContainsKey(_data, makeString(key));
  }

  FORCE_INLINE bool isNull() const {
    return _data == 0;
  }

  FORCE_INLINE size_t size() const {
    return objectSize(_data);
  }

 protected:
  JsonObjectProxy(TData* data) : _data(data) {}
  TData* _data;
};

class JsonObjectConst : public JsonObjectProxy<const JsonObjectData>,
                        public Visitable {
  friend class JsonObject;
  typedef JsonObjectProxy<const JsonObjectData> proxy_type;

 public:
  typedef JsonObjectConstIterator iterator;

  JsonObjectConst() : proxy_type(0) {}
  JsonObjectConst(const JsonObjectData* data) : proxy_type(data) {}

  template <typename Visitor>
  FORCE_INLINE void accept(Visitor& visitor) const {
    if (_data)
      visitor.visitObject(*this);
    else
      visitor.visitNull();
  }

  FORCE_INLINE iterator begin() const {
    if (!_data) return iterator();
    return iterator(_data->head);
  }

  FORCE_INLINE iterator end() const {
    return iterator();
  }

  // Gets the value associated with the specified key.
  //
  // TValue get<TValue>(TKey) const;
  // TKey = const std::string&, const String&
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, JsonArrayConst, JsonObjectConst
  template <typename TValue, typename TKey>
  FORCE_INLINE typename JsonVariantAs<TValue>::type get(const TKey& key) const {
    return get_impl(makeString(key)).template as<TValue>();
  }
  //
  // TValue get<TValue>(TKey) const;
  // TKey = char*, const char*, const FlashStringHelper*
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, JsonArrayConst, JsonObjectConst
  template <typename TValue, typename TKey>
  FORCE_INLINE typename JsonVariantAs<TValue>::type get(TKey* key) const {
    return get_impl(makeString(key)).template as<TValue>();
  }

  //
  // JsonVariantConst operator[](TKey) const;
  // TKey = const std::string&, const String&
  template <typename TKey>
  FORCE_INLINE typename enable_if<IsString<TKey>::value, JsonVariantConst>::type
  operator[](const TKey& key) const {
    return get_impl(makeString(key));
  }
  //
  // JsonVariantConst operator[](TKey) const;
  // TKey = const char*, const char[N], const FlashStringHelper*
  template <typename TKey>
  FORCE_INLINE
      typename enable_if<IsString<TKey*>::value, JsonVariantConst>::type
      operator[](TKey* key) const {
    return get_impl(makeString(key));
  }

  FORCE_INLINE bool operator==(JsonObjectConst rhs) const {
    return objectEquals(_data, rhs._data);
  }

 private:
  template <typename TKey>
  FORCE_INLINE JsonVariantConst get_impl(TKey key) const {
    return JsonVariantConst(objectGet(_data, key));
  }
};

class JsonObject : public JsonObjectProxy<JsonObjectData>, public Visitable {
  typedef JsonObjectProxy<JsonObjectData> proxy_type;

 public:
  typedef JsonObjectIterator iterator;

  FORCE_INLINE JsonObject() : proxy_type(0), _memoryPool(0) {}
  FORCE_INLINE JsonObject(MemoryPool* buf, JsonObjectData* data)
      : proxy_type(data), _memoryPool(buf) {}

  operator JsonVariant() const {
    return JsonVariant(_memoryPool, getVariantData(_data));
  }

  operator JsonObjectConst() const {
    return JsonObjectConst(_data);
  }

  FORCE_INLINE iterator begin() const {
    if (!_data) return iterator();
    return iterator(_memoryPool, _data->head);
  }

  FORCE_INLINE iterator end() const {
    return iterator();
  }

  void clear() const {
    objectClear(_data);
  }

  FORCE_INLINE bool copyFrom(JsonObjectConst src) {
    return objectCopy(_data, src._data, _memoryPool);
  }

  // Creates and adds a JsonArray.
  //
  // JsonArray createNestedArray(TKey);
  // TKey = const std::string&, const String&
  template <typename TKey>
  FORCE_INLINE JsonArray createNestedArray(const TKey& key) const;
  // JsonArray createNestedArray(TKey);
  // TKey = char*, const char*, char[], const char[], const FlashStringHelper*
  template <typename TKey>
  FORCE_INLINE JsonArray createNestedArray(TKey* key) const;

  // Creates and adds a JsonObject.
  //
  // JsonObject createNestedObject(TKey);
  // TKey = const std::string&, const String&
  template <typename TKey>
  FORCE_INLINE JsonObject createNestedObject(const TKey& key) const {
    return set(key).template to<JsonObject>();
  }
  //
  // JsonObject createNestedObject(TKey);
  // TKey = char*, const char*, char[], const char[], const FlashStringHelper*
  template <typename TKey>
  FORCE_INLINE JsonObject createNestedObject(TKey* key) const {
    return set(key).template to<JsonObject>();
  }

  // Gets the value associated with the specified key.
  //
  // TValue get<TValue>(TKey) const;
  // TKey = const std::string&, const String&
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TKey>
  FORCE_INLINE typename JsonVariantAs<TValue>::type get(const TKey& key) const {
    return get_impl(makeString(key)).template as<TValue>();
  }
  //
  // TValue get<TValue>(TKey) const;
  // TKey = char*, const char*, const FlashStringHelper*
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TKey>
  FORCE_INLINE typename JsonVariantAs<TValue>::type get(TKey* key) const {
    return get_impl(makeString(key)).template as<TValue>();
  }

  // Checks the type of the value associated with the specified key.
  //
  //
  // bool is<TValue>(TKey) const;
  // TKey = const std::string&, const String&
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TKey>
  FORCE_INLINE bool is(const TKey& key) const {
    return get_impl(makeString(key)).template is<TValue>();
  }
  //
  // bool is<TValue>(TKey) const;
  // TKey = char*, const char*, const FlashStringHelper*
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TKey>
  FORCE_INLINE bool is(TKey* key) const {
    return get_impl(makeString(key)).template is<TValue>();
  }

  // Gets or sets the value associated with the specified key.
  //
  // JsonObjectSubscript operator[](TKey)
  // TKey = const std::string&, const String&
  template <typename TKey>
  FORCE_INLINE JsonObjectSubscript<const TKey&> operator[](
      const TKey& key) const {
    return JsonObjectSubscript<const TKey&>(*this, key);
  }
  //
  // JsonObjectSubscript operator[](TKey)
  // TKey = char*, const char*, char[], const char[N], const FlashStringHelper*
  template <typename TKey>
  FORCE_INLINE JsonObjectSubscript<TKey*> operator[](TKey* key) const {
    return JsonObjectSubscript<TKey*>(*this, key);
  }

  FORCE_INLINE bool operator==(JsonObject rhs) const {
    return objectEquals(_data, rhs._data);
  }

  FORCE_INLINE void remove(iterator it) const {
    objectRemove(_data, it.internal());
  }

  // Removes the specified key and the associated value.
  //
  // void remove(TKey);
  // TKey = const std::string&, const String&
  template <typename TKey>
  FORCE_INLINE void remove(const TKey& key) const {
    remove_impl(makeString(key));
  }
  //
  // void remove(TKey);
  // TKey = char*, const char*, char[], const char[], const FlashStringHelper*
  template <typename TKey>
  FORCE_INLINE void remove(TKey* key) const {
    remove_impl(makeString(key));
  }

  // Sets the specified key with the specified value.
  //
  // bool set(TKey, TValue);
  // TKey = const std::string&, const String&
  // TValue = bool, long, int, short, float, double, serialized, JsonVariant,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TKey>
  FORCE_INLINE bool set(const TKey& key, const TValue& value) const {
    return set(key).set(value);
  }
  //
  // bool set(TKey, TValue);
  // TKey = const std::string&, const String&
  // TValue = char*, const char*, const FlashStringHelper*
  template <typename TValue, typename TKey>
  FORCE_INLINE bool set(const TKey& key, TValue* value) const {
    return set(key).set(value);
  }
  //
  // bool set(TKey, const TValue&);
  // TKey = char*, const char*, const FlashStringHelper*
  // TValue = bool, long, int, short, float, double, serialized, JsonVariant,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TKey>
  FORCE_INLINE bool set(TKey* key, const TValue& value) const {
    return set(key).set(value);
  }
  //
  // bool set(TKey, TValue);
  // TKey = char*, const char*, const FlashStringHelper*
  // TValue = char*, const char*, const FlashStringHelper*
  template <typename TValue, typename TKey>
  FORCE_INLINE bool set(TKey* key, TValue* value) const {
    return set(key).set(value);
  }

  template <typename TKey>
  FORCE_INLINE JsonVariant set(TKey* key) const {
    return set_impl(makeString(key));
  }

  template <typename TKey>
  FORCE_INLINE JsonVariant set(const TKey& key) const {
    return set_impl(makeString(key));
  }

  FORCE_INLINE JsonVariant set(const StringInMemoryPool& key) const {
    return set_impl(key);
  }

  template <typename Visitor>
  FORCE_INLINE void accept(Visitor& visitor) const {
    JsonObjectConst(_data).accept(visitor);
  }

 private:
  template <typename TStringRef>
  FORCE_INLINE JsonVariant get_impl(TStringRef key) const {
    return JsonVariant(_memoryPool, objectGet(_data, key));
  }

  template <typename TKey>
  FORCE_INLINE JsonVariant set_impl(TKey key) const {
    return JsonVariant(_memoryPool, objectSet(_data, key, _memoryPool));
  }

  template <typename TStringRef>
  FORCE_INLINE void remove_impl(TStringRef key) const {
    objectRemove(_data, objectFindSlot(_data, key));
  }

  MemoryPool* _memoryPool;
};
}  // namespace ARDUINOJSON_NAMESPACE
