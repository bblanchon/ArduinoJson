// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ElementProxy.hpp>
#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Object/JsonObject.hpp>
#include <ArduinoJson/Object/MemberProxy.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>
#include <ArduinoJson/Variant/JsonVariantConst.hpp>
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
    return to<JsonVariant>().set(src.as<JsonVariantConst>());
  }

  template <typename T>
  typename enable_if<!is_base_of<JsonDocument, T>::value, bool>::type set(
      const T& src) {
    return to<JsonVariant>().set(src);
  }

  template <typename T>
  typename VariantTo<T>::type to() {
    clear();
    return getVariant().template to<T>();
  }

  JsonArray createNestedArray() {
    return add().to<JsonArray>();
  }

  // createNestedArray(char*)
  // createNestedArray(const char*)
  // createNestedArray(const __FlashStringHelper*)
  template <typename TChar>
  JsonArray createNestedArray(TChar* key) {
    return operator[](key).template to<JsonArray>();
  }

  // createNestedArray(const std::string&)
  // createNestedArray(const String&)
  template <typename TString>
  JsonArray createNestedArray(const TString& key) {
    return operator[](key).template to<JsonArray>();
  }

  JsonObject createNestedObject() {
    return add().to<JsonObject>();
  }

  // createNestedObject(char*)
  // createNestedObject(const char*)
  // createNestedObject(const __FlashStringHelper*)
  template <typename TChar>
  JsonObject createNestedObject(TChar* key) {
    return operator[](key).template to<JsonObject>();
  }

  // createNestedObject(const std::string&)
  // createNestedObject(const String&)
  template <typename TString>
  JsonObject createNestedObject(const TString& key) {
    return operator[](key).template to<JsonObject>();
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
  FORCE_INLINE typename enable_if<IsString<TString>::value,
                                  MemberProxy<JsonDocument&, TString> >::type
  operator[](const TString& key) {
    return MemberProxy<JsonDocument&, TString>(*this, key);
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
      typename enable_if<IsString<TString>::value, JsonVariantConst>::type
      operator[](const TString& key) const {
    return JsonVariantConst(_data.getMember(adaptString(key)));
  }

  // operator[](char*) const
  // operator[](const char*) const
  // operator[](const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE
      typename enable_if<IsString<TChar*>::value, JsonVariantConst>::type
      operator[](TChar* key) const {
    return JsonVariantConst(_data.getMember(adaptString(key)));
  }

  FORCE_INLINE ElementProxy<JsonDocument&> operator[](size_t index) {
    return ElementProxy<JsonDocument&>(*this, index);
  }

  FORCE_INLINE JsonVariantConst operator[](size_t index) const {
    return JsonVariantConst(_data.getElement(index));
  }

  FORCE_INLINE JsonVariant add() {
    return JsonVariant(&_pool, _data.addElement(&_pool));
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

  FORCE_INLINE operator JsonVariant() {
    return getVariant();
  }

  FORCE_INLINE operator JsonVariantConst() const {
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

  JsonVariant getVariant() {
    return JsonVariant(&_pool, &_data);
  }

  JsonVariantConst getVariant() const {
    return JsonVariantConst(&_data);
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

inline void convertToJson(const JsonDocument& src, JsonVariant dst) {
  dst.set(src.as<JsonVariantConst>());
}

}  // namespace ARDUINOJSON_NAMESPACE
