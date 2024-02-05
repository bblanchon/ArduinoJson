// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Variant/Converter.hpp>
#include <ArduinoJson/Variant/JsonVariantConst.hpp>
#include <ArduinoJson/Variant/VariantOperators.hpp>
#include <ArduinoJson/Variant/VariantTo.hpp>

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
    VariantData::setNull(getOrCreateData(), getResourceManager());
  }

  // Returns true if the value is null or the reference is unbound.
  // https://arduinojson.org/v7/api/jsonvariant/isnull/
  bool isNull() const {
    return VariantData::isNull(getData());
  }

  // Returns true if the reference is unbound.
  bool isUnbound() const {
    return !getData();
  }

  // Casts the value to the specified type.
  // https://arduinojson.org/v7/api/jsonvariant/as/
  template <typename T>

  typename enable_if<!ConverterNeedsWriteableRef<T>::value, T>::type as()
      const {
    return Converter<T>::fromJson(getVariantConst());
  }

  // Casts the value to the specified type.
  // https://arduinojson.org/v7/api/jsonvariant/as/
  template <typename T>
  typename enable_if<ConverterNeedsWriteableRef<T>::value, T>::type as() const;

  template <typename T,
            typename = typename enable_if<!is_same<T, TDerived>::value>::type>
  operator T() const {
    return as<T>();
  }

  // Sets the value to an empty array.
  // https://arduinojson.org/v7/api/jsonvariant/to/
  template <typename T>
  typename enable_if<is_same<T, JsonArray>::value, JsonArray>::type to() const;

  // Sets the value to an empty object.
  // https://arduinojson.org/v7/api/jsonvariant/to/
  template <typename T>
  typename enable_if<is_same<T, JsonObject>::value, JsonObject>::type to()
      const;

  // Sets the value to null.
  // https://arduinojson.org/v7/api/jsonvariant/to/
  template <typename T>
  typename enable_if<is_same<T, JsonVariant>::value, JsonVariant>::type to()
      const;

  // Returns true if the value is of the specified type.
  // https://arduinojson.org/v7/api/jsonvariant/is/
  template <typename T>
  FORCE_INLINE
      typename enable_if<ConverterNeedsWriteableRef<T>::value, bool>::type
      is() const;

  // Returns true if the value is of the specified type.
  // https://arduinojson.org/v7/api/jsonvariant/is/
  template <typename T>
  FORCE_INLINE
      typename enable_if<!ConverterNeedsWriteableRef<T>::value, bool>::type
      is() const {
    return Converter<T>::checkJson(getVariantConst());
  }

  // Copies the specified value.
  // https://arduinojson.org/v7/api/jsonvariant/set/
  template <typename T>
  bool set(const T& value) const;

  // Copies the specified value.
  // https://arduinojson.org/v7/api/jsonvariant/set/
  template <typename T>
  bool set(T* value) const;

  // Returns the size of the array or object.
  // https://arduinojson.org/v7/api/jsonvariant/size/
  size_t size() const {
    return VariantData::size(getData(), getResourceManager());
  }

  // Returns the depth (nesting level) of the value.
  // https://arduinojson.org/v7/api/jsonvariant/nesting/
  size_t nesting() const {
    return VariantData::nesting(getData(), getResourceManager());
  }

  // Appends a new (empty) element to the array.
  // Returns a reference to the new element.
  // https://arduinojson.org/v7/api/jsonvariant/add/
  template <typename T>
  typename enable_if<!is_same<T, JsonVariant>::value, T>::type add() const {
    return add<JsonVariant>().template to<T>();
  }

  // Appends a new (null) element to the array.
  // Returns a reference to the new element.
  // https://arduinojson.org/v7/api/jsonvariant/add/
  template <typename T>
  typename enable_if<is_same<T, JsonVariant>::value, T>::type add() const;

  // Appends a value to the array.
  // https://arduinojson.org/v7/api/jsonvariant/add/
  template <typename T>
  bool add(const T& value) const {
    return add<JsonVariant>().set(value);
  }

  // Appends a value to the array.
  // https://arduinojson.org/v7/api/jsonvariant/add/
  template <typename T>
  bool add(T* value) const {
    return add<JsonVariant>().set(value);
  }

  // Removes an element of the array.
  // https://arduinojson.org/v7/api/jsonvariant/remove/
  void remove(size_t index) const {
    VariantData::removeElement(getData(), index, getResourceManager());
  }

  // Removes a member of the object.
  // https://arduinojson.org/v7/api/jsonvariant/remove/
  template <typename TChar>
  typename enable_if<IsString<TChar*>::value>::type remove(TChar* key) const {
    VariantData::removeMember(getData(), adaptString(key),
                              getResourceManager());
  }

  // Removes a member of the object.
  // https://arduinojson.org/v7/api/jsonvariant/remove/
  template <typename TString>
  typename enable_if<IsString<TString>::value>::type remove(
      const TString& key) const {
    VariantData::removeMember(getData(), adaptString(key),
                              getResourceManager());
  }

  // Gets or sets an array element.
  // https://arduinojson.org/v7/api/jsonvariant/subscript/
  ElementProxy<TDerived> operator[](size_t index) const;

  // Returns true if the object contains the specified key.
  // https://arduinojson.org/v7/api/jsonvariant/containskey/
  template <typename TString>
  typename enable_if<IsString<TString>::value, bool>::type containsKey(
      const TString& key) const;

  // Returns true if the object contains the specified key.
  // https://arduinojson.org/v7/api/jsonvariant/containskey/
  template <typename TChar>
  typename enable_if<IsString<TChar*>::value, bool>::type containsKey(
      TChar* key) const;

  // Gets or sets an object member.
  // https://arduinojson.org/v7/api/jsonvariant/subscript/
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value,
                                  MemberProxy<TDerived, TString>>::type
  operator[](const TString& key) const;

  // Gets or sets an object member.
  // https://arduinojson.org/v7/api/jsonvariant/subscript/
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar*>::value,
                                  MemberProxy<TDerived, TChar*>>::type
  operator[](TChar* key) const;

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

  FORCE_INLINE ArduinoJson::JsonVariant getVariant() const;

  FORCE_INLINE ArduinoJson::JsonVariantConst getVariantConst() const {
    return ArduinoJson::JsonVariantConst(getData(), getResourceManager());
  }

  ArduinoJson::JsonVariant getOrCreateVariant() const;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
