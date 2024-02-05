// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ElementProxy.hpp>
#include <ArduinoJson/Memory/Allocator.hpp>
#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Object/JsonObject.hpp>
#include <ArduinoJson/Object/MemberProxy.hpp>
#include <ArduinoJson/Polyfills/utility.hpp>
#include <ArduinoJson/Variant/JsonVariantConst.hpp>
#include <ArduinoJson/Variant/VariantTo.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// A JSON document.
// https://arduinojson.org/v7/api/jsondocument/
class JsonDocument : public detail::VariantOperators<const JsonDocument&> {
  friend class detail::VariantAttorney;

 public:
  explicit JsonDocument(Allocator* alloc = detail::DefaultAllocator::instance())
      : resources_(alloc) {}

  // Copy-constructor
  JsonDocument(const JsonDocument& src) : JsonDocument(src.allocator()) {
    set(src);
  }

  // Move-constructor
  JsonDocument(JsonDocument&& src)
      : JsonDocument(detail::DefaultAllocator::instance()) {
    swap(*this, src);
  }

  // Construct from variant, array, or object
  template <typename T>
  JsonDocument(const T& src,
               Allocator* alloc = detail::DefaultAllocator::instance(),
               typename detail::enable_if<
                   detail::is_same<T, JsonVariant>::value ||
                   detail::is_same<T, JsonVariantConst>::value ||
                   detail::is_same<T, JsonArray>::value ||
                   detail::is_same<T, JsonArrayConst>::value ||
                   detail::is_same<T, JsonObject>::value ||
                   detail::is_same<T, JsonObjectConst>::value>::type* = 0)
      : JsonDocument(alloc) {
    set(src);
  }

  JsonDocument& operator=(JsonDocument src) {
    swap(*this, src);
    return *this;
  }

  template <typename T>
  JsonDocument& operator=(const T& src) {
    set(src);
    return *this;
  }

  Allocator* allocator() const {
    return resources_.allocator();
  }

  // Reduces the capacity of the memory pool to match the current usage.
  // https://arduinojson.org/v7/api/jsondocument/shrinktofit/
  void shrinkToFit() {
    resources_.shrinkToFit();
  }

  // Casts the root to the specified type.
  // https://arduinojson.org/v7/api/jsondocument/as/
  template <typename T>
  T as() {
    return getVariant().template as<T>();
  }

  // Casts the root to the specified type.
  // https://arduinojson.org/v7/api/jsondocument/as/
  template <typename T>
  T as() const {
    return getVariant().template as<T>();
  }

  // Empties the document and resets the memory pool
  // https://arduinojson.org/v7/api/jsondocument/clear/
  void clear() {
    resources_.clear();
    data_.reset();
  }

  // Returns true if the root is of the specified type.
  // https://arduinojson.org/v7/api/jsondocument/is/
  template <typename T>
  bool is() {
    return getVariant().template is<T>();
  }

  // Returns true if the root is of the specified type.
  // https://arduinojson.org/v7/api/jsondocument/is/
  template <typename T>
  bool is() const {
    return getVariant().template is<T>();
  }

  // Returns true if the root is null.
  // https://arduinojson.org/v7/api/jsondocument/isnull/
  bool isNull() const {
    return getVariant().isNull();
  }

  // Returns trues if the memory pool was too small.
  // https://arduinojson.org/v7/api/jsondocument/overflowed/
  bool overflowed() const {
    return resources_.overflowed();
  }

  // Returns the depth (nesting level) of the array.
  // https://arduinojson.org/v7/api/jsondocument/nesting/
  size_t nesting() const {
    return data_.nesting(&resources_);
  }

  // Returns the number of elements in the root array or object.
  // https://arduinojson.org/v7/api/jsondocument/size/
  size_t size() const {
    return data_.size(&resources_);
  }

  // Copies the specified document.
  // https://arduinojson.org/v7/api/jsondocument/set/
  bool set(const JsonDocument& src) {
    return to<JsonVariant>().set(src.as<JsonVariantConst>());
  }

  // Replaces the root with the specified value.
  // https://arduinojson.org/v7/api/jsondocument/set/
  template <typename T>
  typename detail::enable_if<!detail::is_base_of<JsonDocument, T>::value,
                             bool>::type
  set(const T& src) {
    return to<JsonVariant>().set(src);
  }

  // Clears the document and converts it to the specified type.
  // https://arduinojson.org/v7/api/jsondocument/to/
  template <typename T>
  typename detail::VariantTo<T>::type to() {
    clear();
    return getVariant().template to<T>();
  }

  // Returns true if the root object contains the specified key.
  // https://arduinojson.org/v7/api/jsondocument/containskey/
  template <typename TChar>
  bool containsKey(TChar* key) const {
    return data_.getMember(detail::adaptString(key), &resources_) != 0;
  }

  // Returns true if the root object contains the specified key.
  // https://arduinojson.org/v7/api/jsondocument/containskey/
  template <typename TString>
  bool containsKey(const TString& key) const {
    return data_.getMember(detail::adaptString(key), &resources_) != 0;
  }

  // Gets or sets a root object's member.
  // https://arduinojson.org/v7/api/jsondocument/subscript/
  template <typename TString>
  typename detail::enable_if<detail::IsString<TString>::value,
                             detail::MemberProxy<JsonDocument&, TString>>::type
  operator[](const TString& key) {
    return {*this, key};
  }

  // Gets or sets a root object's member.
  // https://arduinojson.org/v7/api/jsondocument/subscript/
  template <typename TChar>
  typename detail::enable_if<detail::IsString<TChar*>::value,
                             detail::MemberProxy<JsonDocument&, TChar*>>::type
  operator[](TChar* key) {
    return {*this, key};
  }

  // Gets a root object's member.
  // https://arduinojson.org/v7/api/jsondocument/subscript/
  template <typename TString>
  typename detail::enable_if<detail::IsString<TString>::value,
                             JsonVariantConst>::type
  operator[](const TString& key) const {
    return JsonVariantConst(
        data_.getMember(detail::adaptString(key), &resources_), &resources_);
  }

  // Gets a root object's member.
  // https://arduinojson.org/v7/api/jsondocument/subscript/
  template <typename TChar>
  typename detail::enable_if<detail::IsString<TChar*>::value,
                             JsonVariantConst>::type
  operator[](TChar* key) const {
    return JsonVariantConst(
        data_.getMember(detail::adaptString(key), &resources_), &resources_);
  }

  // Gets or sets a root array's element.
  // https://arduinojson.org/v7/api/jsondocument/subscript/
  detail::ElementProxy<JsonDocument&> operator[](size_t index) {
    return {*this, index};
  }

  // Gets a root array's member.
  // https://arduinojson.org/v7/api/jsondocument/subscript/
  JsonVariantConst operator[](size_t index) const {
    return JsonVariantConst(data_.getElement(index, &resources_), &resources_);
  }

  // Appends a new (empty) element to the root array.
  // Returns a reference to the new element.
  // https://arduinojson.org/v7/api/jsondocument/add/
  template <typename T>
  typename detail::enable_if<!detail::is_same<T, JsonVariant>::value, T>::type
  add() {
    return add<JsonVariant>().to<T>();
  }

  // Appends a new (null) element to the root array.
  // Returns a reference to the new element.
  // https://arduinojson.org/v7/api/jsondocument/add/
  template <typename T>
  typename detail::enable_if<detail::is_same<T, JsonVariant>::value, T>::type
  add() {
    return JsonVariant(data_.addElement(&resources_), &resources_);
  }

  // Appends a value to the root array.
  // https://arduinojson.org/v7/api/jsondocument/add/
  template <typename TValue>
  bool add(const TValue& value) {
    return add<JsonVariant>().set(value);
  }

  // Appends a value to the root array.
  // https://arduinojson.org/v7/api/jsondocument/add/
  template <typename TChar>
  bool add(TChar* value) {
    return add<JsonVariant>().set(value);
  }

  // Removes an element of the root array.
  // https://arduinojson.org/v7/api/jsondocument/remove/
  void remove(size_t index) {
    detail::VariantData::removeElement(getData(), index, getResourceManager());
  }

  // Removes a member of the root object.
  // https://arduinojson.org/v7/api/jsondocument/remove/
  template <typename TChar>
  typename detail::enable_if<detail::IsString<TChar*>::value>::type remove(
      TChar* key) {
    detail::VariantData::removeMember(getData(), detail::adaptString(key),
                                      getResourceManager());
  }

  // Removes a member of the root object.
  // https://arduinojson.org/v7/api/jsondocument/remove/
  template <typename TString>

  typename detail::enable_if<detail::IsString<TString>::value>::type remove(
      const TString& key) {
    detail::VariantData::removeMember(getData(), detail::adaptString(key),
                                      getResourceManager());
  }

  operator JsonVariant() {
    return getVariant();
  }

  operator JsonVariantConst() const {
    return getVariant();
  }

  friend void swap(JsonDocument& a, JsonDocument& b) {
    swap(a.resources_, b.resources_);
    swap_(a.data_, b.data_);
  }

  // DEPRECATED: use add<JsonVariant>() instead
  ARDUINOJSON_DEPRECATED("use add<JsonVariant>() instead")
  JsonVariant add() {
    return add<JsonVariant>();
  }

  // DEPRECATED: use add<JsonArray>() instead
  ARDUINOJSON_DEPRECATED("use add<JsonArray>() instead")
  JsonArray createNestedArray() {
    return add<JsonArray>();
  }

  // DEPRECATED: use doc[key].to<JsonArray>() instead
  template <typename TChar>
  ARDUINOJSON_DEPRECATED("use doc[key].to<JsonArray>() instead")
  JsonArray createNestedArray(TChar* key) {
    return operator[](key).template to<JsonArray>();
  }

  // DEPRECATED: use doc[key].to<JsonArray>() instead
  template <typename TString>
  ARDUINOJSON_DEPRECATED("use doc[key].to<JsonArray>() instead")
  JsonArray createNestedArray(const TString& key) {
    return operator[](key).template to<JsonArray>();
  }

  // DEPRECATED: use add<JsonObject>() instead
  ARDUINOJSON_DEPRECATED("use add<JsonObject>() instead")
  JsonObject createNestedObject() {
    return add<JsonObject>();
  }

  // DEPRECATED: use doc[key].to<JsonObject>() instead
  template <typename TChar>
  ARDUINOJSON_DEPRECATED("use doc[key].to<JsonObject>() instead")
  JsonObject createNestedObject(TChar* key) {
    return operator[](key).template to<JsonObject>();
  }

  // DEPRECATED: use doc[key].to<JsonObject>() instead
  template <typename TString>
  ARDUINOJSON_DEPRECATED("use doc[key].to<JsonObject>() instead")
  JsonObject createNestedObject(const TString& key) {
    return operator[](key).template to<JsonObject>();
  }

  // DEPRECATED: always returns zero
  ARDUINOJSON_DEPRECATED("always returns zero")
  size_t memoryUsage() const {
    return 0;
  }

 private:
  JsonVariant getVariant() {
    return JsonVariant(&data_, &resources_);
  }

  JsonVariantConst getVariant() const {
    return JsonVariantConst(&data_, &resources_);
  }

  detail::ResourceManager* getResourceManager() {
    return &resources_;
  }

  detail::VariantData* getData() {
    return &data_;
  }

  const detail::VariantData* getData() const {
    return &data_;
  }

  detail::VariantData* getOrCreateData() {
    return &data_;
  }

  detail::ResourceManager resources_;
  detail::VariantData data_;
};

inline void convertToJson(const JsonDocument& src, JsonVariant dst) {
  dst.set(src.as<JsonVariantConst>());
}

ARDUINOJSON_END_PUBLIC_NAMESPACE
