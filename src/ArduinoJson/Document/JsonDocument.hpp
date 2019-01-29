// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Memory/MemoryPool.hpp"
#include "../Object/ObjectRef.hpp"
#include "../Variant/VariantRef.hpp"
#include "../Variant/VariantTo.hpp"

#include "../Array/ElementProxy.hpp"
#include "../Object/MemberProxy.hpp"

namespace ARDUINOJSON_NAMESPACE {

class JsonDocument : public Visitable {
 public:
  template <typename Visitor>
  void accept(Visitor& visitor) const {
    return getVariant().accept(visitor);
  }

  template <typename T>
  typename VariantAs<T>::type as() {
    return getVariant().template as<T>();
  }

  template <typename T>
  typename VariantConstAs<T>::type as() const {
    return getVariant().template as<T>();
  }

  void clear() {
    _pool.clear();
    _data.setNull();
  }

  template <typename T>
  bool is() const {
    return getVariant().template is<T>();
  }

  bool isNull() const {
    return getVariant().isNull();
  }

  size_t memoryUsage() const {
    return _pool.size();
  }

  size_t nesting() const {
    return _data.nesting();
  }

  size_t capacity() const {
    return _pool.capacity();
  }

  bool set(const JsonDocument& src) {
    return to<VariantRef>().set(src.as<VariantRef>());
  }

  template <typename T>
  typename enable_if<!is_base_of<JsonDocument, T>::value, bool>::type set(
      const T& src) {
    return to<VariantRef>().set(src);
  }

  template <typename T>
  typename VariantTo<T>::type to() {
    clear();
    return getVariant().template to<T>();
  }

  // for internal use only
  MemoryPool& memoryPool() {
    return _pool;
  }

  VariantData& data() {
    return _data;
  }

  ArrayRef createNestedArray() {
    return add().to<ArrayRef>();
  }

  template <typename TKey>
  ArrayRef createNestedArray(TKey* key) {
    return getOrCreate(key).template to<ArrayRef>();
  }

  template <typename TKey>
  ArrayRef createNestedArray(const TKey& key) {
    return getOrCreate(key).template to<ArrayRef>();
  }

  ObjectRef createNestedObject() {
    return add().to<ObjectRef>();
  }

  template <typename TKey>
  ObjectRef createNestedObject(TKey* key) {
    return getOrCreate(key).template to<ObjectRef>();
  }

  template <typename TKey>
  ObjectRef createNestedObject(const TKey& key) {
    return getOrCreate(key).template to<ObjectRef>();
  }

  // MemberProxy operator[](TKey)
  // TKey = const std::string&, const String&
  template <typename TKey>
  FORCE_INLINE
      typename enable_if<IsString<TKey>::value,
                         MemberProxy<JsonDocument&, const TKey&> >::type
      operator[](const TKey& key) {
    return MemberProxy<JsonDocument&, const TKey&>(*this, key);
  }

  // MemberProxy operator[](TKey);
  // TKey = const char*, const char[N], const __FlashStringHelper*
  template <typename TKey>
  FORCE_INLINE typename enable_if<IsString<TKey*>::value,
                                  MemberProxy<JsonDocument&, TKey*> >::type
  operator[](TKey* key) {
    return MemberProxy<JsonDocument&, TKey*>(*this, key);
  }

  // VariantConstRef operator[](TKey) const
  // TKey = const std::string&, const String&
  template <typename TKey>
  FORCE_INLINE typename enable_if<IsString<TKey>::value, VariantConstRef>::type
  operator[](const TKey& key) const {
    return getVariant()[key];
  }

  // VariantConstRef operator[](TKey) const;
  // TKey = const char*, const char[N], const __FlashStringHelper*
  template <typename TKey>
  FORCE_INLINE typename enable_if<IsString<TKey*>::value, VariantConstRef>::type
  operator[](TKey* key) const {
    return getVariant()[key];
  }

  FORCE_INLINE ElementProxy<JsonDocument&> operator[](size_t index) {
    return ElementProxy<JsonDocument&>(*this, index);
  }

  FORCE_INLINE VariantConstRef operator[](size_t index) const {
    return VariantConstRef(_data.get(index));
  }

  FORCE_INLINE VariantRef get(size_t index) {
    return VariantRef(&_pool, _data.get(index));
  }

  template <typename TKey>
  FORCE_INLINE VariantRef get(TKey* key) {
    return VariantRef(&_pool, _data.get(wrapString(key)));
  }

  template <typename TKey>
  FORCE_INLINE typename enable_if<IsString<TKey>::value, VariantRef>::type get(
      const TKey& key) {
    return VariantRef(&_pool, _data.get(wrapString(key)));
  }

  template <typename TKey>
  FORCE_INLINE VariantRef getOrCreate(TKey* key) {
    return VariantRef(&_pool, _data.getOrCreate(wrapString(key), &_pool));
  }

  template <typename TKey>
  FORCE_INLINE VariantRef getOrCreate(const TKey& key) {
    return VariantRef(&_pool, _data.getOrCreate(wrapString(key), &_pool));
  }

  FORCE_INLINE VariantRef add() {
    return VariantRef(&_pool, _data.add(&_pool));
  }
  //
  // bool add(TValue);
  // TValue = bool, long, int, short, float, double, serialized, VariantRef,
  //          std::string, String, ObjectRef
  template <typename T>
  FORCE_INLINE bool add(const T& value) {
    return add().set(value);
  }
  //
  // bool add(TValue);
  // TValue = char*, const char*, const __FlashStringHelper*
  template <typename T>
  FORCE_INLINE bool add(T* value) {
    return add().set(value);
  }

 protected:
  JsonDocument(MemoryPool pool) : _pool(pool) {
    _data.setNull();
  }

  JsonDocument(char* buf, size_t capa) : _pool(buf, capa) {
    _data.setNull();
  }

  void replacePool(MemoryPool pool) {
    _pool = pool;
  }

 private:
  VariantRef getVariant() {
    return VariantRef(&_pool, &_data);
  }

  VariantConstRef getVariant() const {
    return VariantConstRef(&_data);
  }

  MemoryPool _pool;
  VariantData _data;
};

}  // namespace ARDUINOJSON_NAMESPACE
