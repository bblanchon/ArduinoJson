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

  // createNestedArray(char*)
  // createNestedArray(const char*)
  // createNestedArray(const __FlashStringHelper*)
  template <typename TChar>
  ArrayRef createNestedArray(TChar* key) {
    return getOrCreate(key).template to<ArrayRef>();
  }

  // createNestedArray(const std::string&)
  // createNestedArray(const String&)
  template <typename TString>
  ArrayRef createNestedArray(const TString& key) {
    return getOrCreate(key).template to<ArrayRef>();
  }

  ObjectRef createNestedObject() {
    return add().to<ObjectRef>();
  }

  // createNestedObject(char*)
  // createNestedObject(const char*)
  // createNestedObject(const __FlashStringHelper*)
  template <typename TChar>
  ObjectRef createNestedObject(TChar* key) {
    return getOrCreate(key).template to<ObjectRef>();
  }

  // createNestedObject(const std::string&)
  // createNestedObject(const String&)
  template <typename TString>
  ObjectRef createNestedObject(const TString& key) {
    return getOrCreate(key).template to<ObjectRef>();
  }

  // operator[](const std::string&)
  // operator[](const String&)
  template <typename TString>
  FORCE_INLINE
      typename enable_if<IsString<TString>::value,
                         MemberProxy<JsonDocument&, const TString&> >::type
      operator[](const TString& key) {
    return MemberProxy<JsonDocument&, const TString&>(*this, key);
  }

  // operator[](char*)
  // operator[](const char*)
  // operator[](const __FlashStringHelper*)
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar*>::value,
                                  MemberProxy<JsonDocument&, TChar*> >::type
  operator[](TChar* key) {
    return MemberProxy<JsonDocument&, TChar*>(*this, key);
  }

  // operator[](const std::string&) const
  // operator[](const String&) const
  template <typename TString>
  FORCE_INLINE
      typename enable_if<IsString<TString>::value, VariantConstRef>::type
      operator[](const TString& key) const {
    return getVariant()[key];
  }

  // operator[](char*) const
  // operator[](const char*) const
  // operator[](const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE
      typename enable_if<IsString<TChar*>::value, VariantConstRef>::type
      operator[](TChar* key) const {
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

  // get(char*) const
  // get(const char*) const
  // get(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE VariantRef get(TChar* key) {
    return VariantRef(&_pool, _data.get(adaptString(key)));
  }

  // get(const std::string&) const
  // get(const String&) const
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value, VariantRef>::type
  get(const TString& key) {
    return VariantRef(&_pool, _data.get(adaptString(key)));
  }

  // getOrCreate(char*)
  // getOrCreate(const char*)
  // getOrCreate(const __FlashStringHelper*)
  template <typename TChar>
  FORCE_INLINE VariantRef getOrCreate(TChar* key) {
    return VariantRef(&_pool, _data.getOrCreate(adaptString(key), &_pool));
  }

  // getOrCreate(const std::string&)
  // getOrCreate(const String&)
  template <typename TString>
  FORCE_INLINE VariantRef getOrCreate(const TString& key) {
    return VariantRef(&_pool, _data.getOrCreate(adaptString(key), &_pool));
  }

  FORCE_INLINE VariantRef add() {
    return VariantRef(&_pool, _data.add(&_pool));
  }

  template <typename TValue>
  FORCE_INLINE bool add(const TValue& value) {
    return add().set(value);
  }

  // add(char*) const
  // add(const char*) const
  // add(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE bool add(TChar* value) {
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
