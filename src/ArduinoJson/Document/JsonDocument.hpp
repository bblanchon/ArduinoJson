// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ElementProxy.hpp>
#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Object/MemberProxy.hpp>
#include <ArduinoJson/Object/ObjectRef.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>
#include <ArduinoJson/Variant/VariantConstRef.hpp>
#include <ArduinoJson/Variant/VariantTo.hpp>

namespace ARDUINOJSON_NAMESPACE {

class JsonDocument : public VariantOperators<const JsonDocument&> {
  friend class VariantAttorney;

 public:
  template <typename T>
  T as() {
    return getVariant().template as<T>();
  }

  template <typename T>
  T as() const {
    return getVariant().template as<T>();
  }

  void clear() {
    _pool.clear();
    _data.init();
  }

  template <typename T>
  bool is() {
    return getVariant().template is<T>();
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

  bool overflowed() const {
    return _pool.overflowed();
  }

  size_t nesting() const {
    return variantNesting(&_data);
  }

  size_t capacity() const {
    return _pool.capacity();
  }

  size_t size() const {
    return _data.size();
  }

  bool set(const JsonDocument& src) {
    return to<VariantRef>().set(src.as<VariantConstRef>());
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

  ArrayRef createNestedArray() {
    return add().to<ArrayRef>();
  }

  // createNestedArray(char*)
  // createNestedArray(const char*)
  // createNestedArray(const __FlashStringHelper*)
  template <typename TChar>
  ArrayRef createNestedArray(TChar* key) {
    return operator[](key).template to<ArrayRef>();
  }

  // createNestedArray(const std::string&)
  // createNestedArray(const String&)
  template <typename TString>
  ArrayRef createNestedArray(const TString& key) {
    return operator[](key).template to<ArrayRef>();
  }

  ObjectRef createNestedObject() {
    return add().to<ObjectRef>();
  }

  // createNestedObject(char*)
  // createNestedObject(const char*)
  // createNestedObject(const __FlashStringHelper*)
  template <typename TChar>
  ObjectRef createNestedObject(TChar* key) {
    return operator[](key).template to<ObjectRef>();
  }

  // createNestedObject(const std::string&)
  // createNestedObject(const String&)
  template <typename TString>
  ObjectRef createNestedObject(const TString& key) {
    return operator[](key).template to<ObjectRef>();
  }

  // containsKey(char*) const
  // containsKey(const char*) const
  // containsKey(const __FlashStringHelper*) const
  template <typename TChar>
  bool containsKey(TChar* key) const {
    return _data.getMember(adaptString(key)) != 0;
  }

  // containsKey(const std::string&) const
  // containsKey(const String&) const
  template <typename TString>
  bool containsKey(const TString& key) const {
    return _data.getMember(adaptString(key)) != 0;
  }

  // operator[](const std::string&)
  // operator[](const String&)
  template <typename TString>
  FORCE_INLINE typename enable_if<
      IsString<TString>::value,
      VariantProxy<MemberDataSource<JsonDocument&, TString> > >::type
  operator[](const TString& key) {
    return VariantProxy<MemberDataSource<JsonDocument&, TString> >(
        MemberDataSource<JsonDocument&, TString>(*this, key));
  }

  // operator[](char*)
  // operator[](const char*)
  // operator[](const __FlashStringHelper*)
  template <typename TChar>
  FORCE_INLINE typename enable_if<
      IsString<TChar*>::value,
      VariantProxy<MemberDataSource<JsonDocument&, TChar*> > >::type
  operator[](TChar* key) {
    return VariantProxy<MemberDataSource<JsonDocument&, TChar*> >(
        MemberDataSource<JsonDocument&, TChar*>(*this, key));
  }

  // operator[](const std::string&) const
  // operator[](const String&) const
  template <typename TString>
  FORCE_INLINE
      typename enable_if<IsString<TString>::value, VariantConstRef>::type
      operator[](const TString& key) const {
    return VariantConstRef(_data.getMember(adaptString(key)));
  }

  // operator[](char*) const
  // operator[](const char*) const
  // operator[](const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE
      typename enable_if<IsString<TChar*>::value, VariantConstRef>::type
      operator[](TChar* key) const {
    return VariantConstRef(_data.getMember(adaptString(key)));
  }

  FORCE_INLINE VariantProxy<ElementDataSource<JsonDocument&> > operator[](
      size_t index) {
    return VariantProxy<ElementDataSource<JsonDocument&> >(
        ElementDataSource<JsonDocument&>(*this, index));
  }

  FORCE_INLINE VariantConstRef operator[](size_t index) const {
    return VariantConstRef(_data.getElement(index));
  }

  FORCE_INLINE VariantRef add() {
    return VariantRef(&_pool, _data.addElement(&_pool));
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

  FORCE_INLINE void remove(size_t index) {
    _data.remove(index);
  }
  // remove(char*)
  // remove(const char*)
  // remove(const __FlashStringHelper*)
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar*>::value>::type remove(
      TChar* key) {
    _data.remove(adaptString(key));
  }
  // remove(const std::string&)
  // remove(const String&)
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value>::type remove(
      const TString& key) {
    _data.remove(adaptString(key));
  }

  FORCE_INLINE operator VariantRef() {
    return getVariant();
  }

  FORCE_INLINE operator VariantConstRef() const {
    return getVariant();
  }

 protected:
  JsonDocument() : _pool(0, 0) {
    _data.init();
  }

  JsonDocument(MemoryPool pool) : _pool(pool) {
    _data.init();
  }

  JsonDocument(char* buf, size_t capa) : _pool(buf, capa) {
    _data.init();
  }

  ~JsonDocument() {}

  void replacePool(MemoryPool pool) {
    _pool = pool;
  }

  VariantRef getVariant() {
    return VariantRef(&_pool, &_data);
  }

  VariantConstRef getVariant() const {
    return VariantConstRef(&_data);
  }

  MemoryPool _pool;
  VariantData _data;

 private:
  JsonDocument(const JsonDocument&);
  JsonDocument& operator=(const JsonDocument&);

 protected:
  MemoryPool* getPool() {
    return &_pool;
  }

  VariantData* getData() {
    return &_data;
  }

  const VariantData* getData() const {
    return &_data;
  }

  VariantData* getOrCreateData() {
    return &_data;
  }
};

inline void convertToJson(const JsonDocument& src, VariantRef dst) {
  dst.set(src.as<VariantConstRef>());
}

}  // namespace ARDUINOJSON_NAMESPACE
