// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Variant/Converter.hpp>
#include <ArduinoJson/Variant/JsonVariantConst.hpp>
#include <ArduinoJson/Variant/VariantOperators.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE
class JsonVariant;
ARDUINOJSON_END_PUBLIC_NAMESPACE

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE
template <typename>
class ElementProxy;

template <typename, typename>
class MemberProxy;

template <typename TDerived>
class VariantRefBase : public VariantTag {
  friend class VariantAttorney;

 public:
  // Sets the value to null.
  // https://arduinojson.org/v7/api/jsonvariant/clear/
  void clear() const {
    getOrCreateVariantImpl().clear();
  }

  // Returns true if the value is null or the reference is unbound.
  // https://arduinojson.org/v7/api/jsonvariant/isnull/
  bool isNull() const {
    return getVariantImpl().isNull();
  }

  // Returns true if the reference is unbound.
  bool isUnbound() const {
    return !getData();
  }

  // Casts the value to the specified type.
  // https://arduinojson.org/v7/api/jsonvariant/as/
  template <typename T>
  T as() const;

  template <typename T, enable_if_t<!is_same<T, TDerived>::value, int> = 0>
  operator T() const {
    return as<T>();
  }

  // Sets the value to an empty array.
  // https://arduinojson.org/v7/api/jsonvariant/to/
  template <typename T, enable_if_t<is_same<T, JsonArray>::value, int> = 0>
  JsonArray to() const;

  // Sets the value to an empty object.
  // https://arduinojson.org/v7/api/jsonvariant/to/
  template <typename T, enable_if_t<is_same<T, JsonObject>::value, int> = 0>
  JsonObject to() const;

  // Sets the value to null.
  // https://arduinojson.org/v7/api/jsonvariant/to/
  template <typename T, enable_if_t<is_same<T, JsonVariant>::value, int> = 0>
  JsonVariant to() const;

  // Returns true if the value is of the specified type.
  // https://arduinojson.org/v7/api/jsonvariant/is/
  template <typename T>
  FORCE_INLINE bool is() const;

  // Copies the specified value.
  // https://arduinojson.org/v7/api/jsonvariant/set/
  template <typename T>
  bool set(const T& value) const {
    using TypeForConverter = remove_cv_t<remove_reference_t<T>>;
    return doSet<Converter<TypeForConverter>>(value);
  }

  // Copies the specified value.
  // https://arduinojson.org/v7/api/jsonvariant/set/
  template <typename T>
  bool set(T* value) const {
    return doSet<Converter<T*>>(value);
  }

  // Returns the size of the array or object.
  // https://arduinojson.org/v7/api/jsonvariant/size/
  size_t size() const {
    return getVariantImpl().size();
  }

  // Returns the depth (nesting level) of the value.
  // https://arduinojson.org/v7/api/jsonvariant/nesting/
  size_t nesting() const {
    return getVariantImpl().nesting();
  }

  // Appends a new (empty) element to the array.
  // Returns a reference to the new element.
  // https://arduinojson.org/v7/api/jsonvariant/add/
  template <typename T, enable_if_t<!is_same<T, JsonVariant>::value, int> = 0>
  T add() const {
    return add<JsonVariant>().template to<T>();
  }

  // Appends a new (null) element to the array.
  // Returns a reference to the new element.
  // https://arduinojson.org/v7/api/jsonvariant/add/
  template <typename T, enable_if_t<is_same<T, JsonVariant>::value, int> = 0>
  T add() const;

  // Appends a value to the array.
  // https://arduinojson.org/v7/api/jsonvariant/add/
  template <typename T>
  bool add(const T& value) const {
    return getOrCreateArray().add(value);
  }

  // Appends a value to the array.
  // https://arduinojson.org/v7/api/jsonvariant/add/
  template <typename T>
  bool add(T* value) const {
    return getOrCreateArray().add(value);
  }

  // Removes an element of the array.
  // https://arduinojson.org/v7/api/jsonvariant/remove/
  void remove(size_t index) const {
    getVariantImpl().removeElement(index);
  }

  // Removes a member of the object.
  // https://arduinojson.org/v7/api/jsonvariant/remove/
  template <typename TChar, enable_if_t<IsString<TChar*>::value, int> = 0>
  void remove(TChar* key) const {
    getVariantImpl().removeMember(adaptString(key));
  }

  // Removes a member of the object.
  // https://arduinojson.org/v7/api/jsonvariant/remove/
  template <typename TString, enable_if_t<IsString<TString>::value, int> = 0>
  void remove(const TString& key) const {
    getVariantImpl().removeMember(adaptString(key));
  }

  // Removes a member of the object or an element of the array.
  // https://arduinojson.org/v7/api/jsonvariant/remove/
  template <typename TVariant, enable_if_t<IsVariant<TVariant>::value, int> = 0>
  void remove(const TVariant& key) const {
    if (key.template is<size_t>())
      remove(key.template as<size_t>());
    else
      remove(key.template as<const char*>());
  }

  // Gets or sets an array element.
  // https://arduinojson.org/v7/api/jsonvariant/subscript/
  ElementProxy<TDerived> operator[](size_t index) const;

  // DEPRECATED: use obj[key].is<T>() instead
  // https://arduinojson.org/v7/api/jsonvariant/containskey/
  template <typename TString, enable_if_t<IsString<TString>::value, int> = 0>
  ARDUINOJSON_DEPRECATED("use obj[key].is<T>() instead")
  bool containsKey(const TString& key) const;

  // DEPRECATED: use obj["key"].is<T>() instead
  // https://arduinojson.org/v7/api/jsonvariant/containskey/
  template <typename TChar, enable_if_t<IsString<TChar*>::value, int> = 0>
  ARDUINOJSON_DEPRECATED("use obj[\"key\"].is<T>() instead")
  bool containsKey(TChar* key) const;

  // DEPRECATED: use obj[key].is<T>() instead
  // https://arduinojson.org/v7/api/jsonvariant/containskey/
  template <typename TVariant, enable_if_t<IsVariant<TVariant>::value, int> = 0>
  ARDUINOJSON_DEPRECATED("use obj[key].is<T>() instead")
  bool containsKey(const TVariant& key) const;

  // Gets or sets an object member.
  // https://arduinojson.org/v7/api/jsonvariant/subscript/
  template <typename TString, enable_if_t<IsString<TString>::value, int> = 0>
  FORCE_INLINE MemberProxy<TDerived, AdaptedString<TString>> operator[](
      const TString& key) const;

  // Gets or sets an object member.
  // https://arduinojson.org/v7/api/jsonvariant/subscript/
  template <typename TChar, enable_if_t<IsString<TChar*>::value, int> = 0>
  FORCE_INLINE MemberProxy<TDerived, AdaptedString<TChar*>> operator[](
      TChar* key) const;

  // Gets an object member or an array element.
  // https://arduinojson.org/v7/api/jsonvariant/subscript/
  template <typename TVariant, enable_if_t<IsVariant<TVariant>::value, int> = 0>
  JsonVariantConst operator[](const TVariant& key) const {
    if (key.template is<size_t>())
      return operator[](key.template as<size_t>());
    else
      return operator[](key.template as<JsonString>());
  }

  // DEPRECATED: use add<JsonVariant>() instead
  ARDUINOJSON_DEPRECATED("use add<JsonVariant>() instead")
  JsonVariant add() const;

  // DEPRECATED: use add<JsonArray>() instead
  ARDUINOJSON_DEPRECATED("use add<JsonArray>() instead")
  JsonArray createNestedArray() const;

  // DEPRECATED: use var[key].to<JsonArray>() instead
  template <typename TChar>
  ARDUINOJSON_DEPRECATED("use var[key].to<JsonArray>() instead")
  JsonArray createNestedArray(TChar* key) const;

  // DEPRECATED: use var[key].to<JsonArray>() instead
  template <typename TString>
  ARDUINOJSON_DEPRECATED("use var[key].to<JsonArray>() instead")
  JsonArray createNestedArray(const TString& key) const;

  // DEPRECATED: use add<JsonObject>() instead
  ARDUINOJSON_DEPRECATED("use add<JsonObject>() instead")
  JsonObject createNestedObject() const;

  // DEPRECATED: use var[key].to<JsonObject>() instead
  template <typename TChar>
  ARDUINOJSON_DEPRECATED("use var[key].to<JsonObject>() instead")
  JsonObject createNestedObject(TChar* key) const;

  // DEPRECATED: use var[key].to<JsonObject>() instead
  template <typename TString>
  ARDUINOJSON_DEPRECATED("use var[key].to<JsonObject>() instead")
  JsonObject createNestedObject(const TString& key) const;

  // DEPRECATED: always returns zero
  ARDUINOJSON_DEPRECATED("always returns zero")
  size_t memoryUsage() const {
    return 0;
  }

  // DEPRECATED: performs a deep copy
  ARDUINOJSON_DEPRECATED("performs a deep copy")
  void shallowCopy(JsonVariantConst src) const {
    set(src);
  }

 private:
  TDerived& derived() {
    return static_cast<TDerived&>(*this);
  }

  const TDerived& derived() const {
    return static_cast<const TDerived&>(*this);
  }

  ResourceManager* getResourceManager() const {
    return VariantAttorney::getResourceManager(derived());
  }

  VariantData* getData() const {
    return VariantAttorney::getData(derived());
  }

  VariantData* getOrCreateData() const {
    return VariantAttorney::getOrCreateData(derived());
  }

  VariantImpl getVariantImpl() const {
    return VariantImpl(getData(), getResourceManager());
  }

  VariantImpl getOrCreateVariantImpl() const {
    return VariantImpl(getOrCreateData(), getResourceManager());
  }

  JsonArray getOrCreateArray() const;

  FORCE_INLINE ArduinoJson::JsonVariant getVariant() const;

  FORCE_INLINE ArduinoJson::JsonVariantConst getVariantConst() const {
    return ArduinoJson::JsonVariantConst(getData(), getResourceManager());
  }

  template <typename T>
  FORCE_INLINE enable_if_t<is_same<T, JsonVariantConst>::value, T> getVariant()
      const {
    return getVariantConst();
  }

  template <typename T>
  FORCE_INLINE enable_if_t<is_same<T, JsonVariant>::value, T> getVariant()
      const {
    return getVariant();
  }

  template <typename TConverter, typename T>
  bool doSet(const T& value) const {
    return doSet<TConverter>(
        value, is_same<typename function_traits<
                           decltype(&TConverter::toJson)>::return_type,
                       bool>{});
  }

  template <typename TConverter, typename T>
  bool doSet(const T& value, false_type) const;

  template <typename TConverter, typename T>
  bool doSet(const T& value, true_type) const;

  ArduinoJson::JsonVariant getOrCreateVariant() const;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
