// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ElementProxy.hpp>
#include <ArduinoJson/Memory/Allocator.hpp>
#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Object/JsonObject.hpp>
#include <ArduinoJson/Object/MemberProxy.hpp>
#include <ArduinoJson/Polyfills/utility.hpp>
#include <ArduinoJson/Variant/JsonVariantConst.hpp>

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
  template <typename T,
            detail::enable_if_t<detail::IsVariant<T>::value ||
                                    detail::is_same<T, JsonArray>::value ||
                                    detail::is_same<T, JsonArrayConst>::value ||
                                    detail::is_same<T, JsonObject>::value ||
                                    detail::is_same<T, JsonObjectConst>::value,
                                int> = 0>
  JsonDocument(const T& src,
               Allocator* alloc = detail::DefaultAllocator::instance())
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
    data_.type = detail::VariantType::Null;
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
    return getVariantImpl().nesting();
  }

  // Returns the number of elements in the root array or object.
  // https://arduinojson.org/v7/api/jsondocument/size/
  size_t size() const {
    return getVariantImpl().size();
  }

  // Copies the specified document.
  // https://arduinojson.org/v7/api/jsondocument/set/
  bool set(const JsonDocument& src) {
    return to<JsonVariant>().set(src.as<JsonVariantConst>());
  }

  // Replaces the root with the specified value.
  // https://arduinojson.org/v7/api/jsondocument/set/
  template <
      typename T,
      detail::enable_if_t<!detail::is_base_of<JsonDocument, T>::value, int> = 0>
  bool set(const T& src) {
    return to<JsonVariant>().set(src);
  }

  // Replaces the root with the specified value.
  // https://arduinojson.org/v7/api/jsondocument/set/
  template <typename TChar>
  bool set(TChar* src) {
    return to<JsonVariant>().set(src);
  }

  // Sets the document to an empty array.
  // https://arduinojson.org/v7/api/jsondocument/to/
  template <typename T,
            detail::enable_if_t<detail::is_same<T, JsonArray>::value, int> = 0>
  JsonArray to() {
    clear();
    data_.toArray();
    return JsonArray(&data_, &resources_);
  }

  // Sets the document to an empty object.
  // https://arduinojson.org/v7/api/jsondocument/to/
  template <typename T,
            detail::enable_if_t<detail::is_same<T, JsonObject>::value, int> = 0>
  JsonObject to() {
    clear();
    data_.toObject();
    return JsonObject(&data_, &resources_);
  }

  // Sets the document to null.
  // https://arduinojson.org/v7/api/jsondocument/to/
  template <typename T, detail::enable_if_t<
                            detail::is_same<T, JsonVariant>::value, int> = 0>
  JsonVariant to() {
    clear();
    return JsonVariant(&data_, &resources_);
  }

  // DEPRECATED: use obj["key"].is<T>() instead
  // https://arduinojson.org/v7/api/jsondocument/containskey/
  template <typename TChar>
  ARDUINOJSON_DEPRECATED("use doc[\"key\"].is<T>() instead")
  bool containsKey(TChar* key) const {
    return getVariantImpl().getMember(detail::adaptString(key)) != 0;
  }

  // DEPRECATED: use obj[key].is<T>() instead
  // https://arduinojson.org/v7/api/jsondocument/containskey/
  template <typename TString,
            detail::enable_if_t<detail::IsString<TString>::value, int> = 0>
  ARDUINOJSON_DEPRECATED("use doc[key].is<T>() instead")
  bool containsKey(const TString& key) const {
    return getVariantImpl().getMember(detail::adaptString(key)) != 0;
  }

  // DEPRECATED: use obj[key].is<T>() instead
  // https://arduinojson.org/v7/api/jsondocument/containskey/
  template <typename TVariant,
            detail::enable_if_t<detail::IsVariant<TVariant>::value, int> = 0>
  ARDUINOJSON_DEPRECATED("use doc[key].is<T>() instead")
  bool containsKey(const TVariant& key) const {
    return containsKey(key.template as<const char*>());
  }

  // Gets or sets a root object's member.
  // https://arduinojson.org/v7/api/jsondocument/subscript/
  template <typename TString,
            detail::enable_if_t<detail::IsString<TString>::value, int> = 0>
  detail::MemberProxy<JsonDocument&, detail::AdaptedString<TString>> operator[](
      const TString& key) {
    return {*this, detail::adaptString(key)};
  }

  // Gets or sets a root object's member.
  // https://arduinojson.org/v7/api/jsondocument/subscript/
  template <typename TChar,
            detail::enable_if_t<detail::IsString<TChar*>::value, int> = 0>
  detail::MemberProxy<JsonDocument&, detail::AdaptedString<TChar*>> operator[](
      TChar* key) {
    return {*this, detail::adaptString(key)};
  }

  // Gets a root object's member.
  // https://arduinojson.org/v7/api/jsondocument/subscript/
  template <typename TString,
            detail::enable_if_t<detail::IsString<TString>::value, int> = 0>
  JsonVariantConst operator[](const TString& key) const {
    return JsonVariantConst(
        getVariantImpl().getMember(detail::adaptString(key)), &resources_);
  }

  // Gets a root object's member.
  // https://arduinojson.org/v7/api/jsondocument/subscript/
  template <typename TChar,
            detail::enable_if_t<detail::IsString<TChar*>::value, int> = 0>
  JsonVariantConst operator[](TChar* key) const {
    return JsonVariantConst(
        getVariantImpl().getMember(detail::adaptString(key)), &resources_);
  }

  // Gets or sets a root array's element.
  // https://arduinojson.org/v7/api/jsondocument/subscript/
  template <typename T,
            detail::enable_if_t<detail::is_integral<T>::value, int> = 0>
  detail::ElementProxy<JsonDocument&> operator[](T index) {
    return {*this, size_t(index)};
  }

  // Gets a root array's member.
  // https://arduinojson.org/v7/api/jsondocument/subscript/
  JsonVariantConst operator[](size_t index) const {
    return JsonVariantConst(getVariantImpl().getElement(index), &resources_);
  }

  // Gets or sets a root object's member.
  // https://arduinojson.org/v7/api/jsondocument/subscript/
  template <typename TVariant,
            detail::enable_if_t<detail::IsVariant<TVariant>::value, int> = 0>
  JsonVariantConst operator[](const TVariant& key) const {
    if (key.template is<JsonString>())
      return operator[](key.template as<JsonString>());
    if (key.template is<size_t>())
      return operator[](key.template as<size_t>());
    return {};
  }

  // Appends a new (empty) element to the root array.
  // Returns a reference to the new element.
  // https://arduinojson.org/v7/api/jsondocument/add/
  template <typename T, detail::enable_if_t<
                            !detail::is_same<T, JsonVariant>::value, int> = 0>
  T add() {
    return add<JsonVariant>().to<T>();
  }

  // Appends a new (null) element to the root array.
  // Returns a reference to the new element.
  // https://arduinojson.org/v7/api/jsondocument/add/
  template <typename T, detail::enable_if_t<
                            detail::is_same<T, JsonVariant>::value, int> = 0>
  JsonVariant add() {
    return getOrCreateArray().add<T>();
  }

  // Appends a value to the root array.
  // https://arduinojson.org/v7/api/jsondocument/add/
  template <typename TValue>
  bool add(const TValue& value) {
    return getOrCreateArray().add(value);
  }

  // Appends a value to the root array.
  // https://arduinojson.org/v7/api/jsondocument/add/
  template <typename TChar>
  bool add(TChar* value) {
    return getOrCreateArray().add(value);
  }

  // Removes an element of the root array.
  // https://arduinojson.org/v7/api/jsondocument/remove/
  template <typename T,
            detail::enable_if_t<detail::is_integral<T>::value, int> = 0>
  void remove(T index) {
    getVariantImpl().removeElement(size_t(index));
  }

  // Removes a member of the root object.
  // https://arduinojson.org/v7/api/jsondocument/remove/
  template <typename TChar,
            detail::enable_if_t<detail::IsString<TChar*>::value, int> = 0>
  void remove(TChar* key) {
    getVariantImpl().removeMember(detail::adaptString(key));
  }

  // Removes a member of the root object.
  // https://arduinojson.org/v7/api/jsondocument/remove/
  template <typename TString,
            detail::enable_if_t<detail::IsString<TString>::value, int> = 0>
  void remove(const TString& key) {
    getVariantImpl().removeMember(detail::adaptString(key));
  }

  // Removes a member of the root object or an element of the root array.
  // https://arduinojson.org/v7/api/jsondocument/remove/
  template <typename TVariant,
            detail::enable_if_t<detail::IsVariant<TVariant>::value, int> = 0>
  void remove(const TVariant& key) {
    if (key.template is<const char*>())
      remove(key.template as<const char*>());
    if (key.template is<size_t>())
      remove(key.template as<size_t>());
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
  detail::VariantImpl getVariantImpl() const {
    return detail::VariantImpl(&data_, &resources_);
  }

  JsonArray getOrCreateArray() const {
    if (data_.type == detail::VariantType::Null)
      data_.toArray();
    return JsonArray(&data_, &resources_);
  }

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

  mutable detail::ResourceManager resources_;
  mutable detail::VariantData data_;
};

inline bool convertToJson(const JsonDocument& src, JsonVariant dst) {
  return dst.set(src.as<JsonVariantConst>());
}

ARDUINOJSON_END_PUBLIC_NAMESPACE
