// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Variant/Converter.hpp>
#include <ArduinoJson/Variant/JsonVariantConst.hpp>
#include <ArduinoJson/Variant/VariantOperators.hpp>
#include <ArduinoJson/Variant/VariantTo.hpp>

namespace ARDUINOJSON_NAMESPACE {

class JsonVariant;

template <typename>
class ElementProxy;

template <typename, typename>
class MemberProxy;

template <typename TDerived>
class VariantRefBase : public VariantTag {
  friend class VariantAttorney;

 public:
  // Sets the value to null.
  // ⚠️ Doesn't release the memory associated with the previous value.
  // https://arduinojson.org/v6/api/jsonvariant/clear/
  FORCE_INLINE void clear() const {
    variantSetNull(getData());
  }

  // Returns true if the value is null or the reference is unbound.
  // https://arduinojson.org/v6/api/jsonvariant/isnull/
  FORCE_INLINE bool isNull() const {
    return variantIsNull(getData());
  }

  // Returns true if the reference is unbound.
  FORCE_INLINE bool isUnbound() const {
    return !getData();
  }

  // Casts the value to the specified type.
  // https://arduinojson.org/v6/api/jsonvariant/as/
  template <typename T>
  FORCE_INLINE typename enable_if<!is_same<T, char*>::value &&
                                      !is_same<T, char>::value &&
                                      !ConverterNeedsWriteableRef<T>::value,
                                  T>::type
  as() const {
    return Converter<T>::fromJson(getVariantConst());
  }

  // Casts the value to the specified type.
  // https://arduinojson.org/v6/api/jsonvariant/as/
  template <typename T>
  FORCE_INLINE typename enable_if<ConverterNeedsWriteableRef<T>::value, T>::type
  as() const {
    return Converter<T>::fromJson(getVariant());
  }

  template <typename T>
  FORCE_INLINE operator T() const {
    return as<T>();
  }

  // Sets the value to an empty array.
  // ⚠️ Doesn't release the memory associated with the previous value.
  // https://arduinojson.org/v6/api/jsonvariant/to/
  template <typename T>
  typename enable_if<is_same<T, JsonArray>::value, JsonArray>::type to() const;

  // Sets the value to an empty object.
  // ⚠️ Doesn't release the memory associated with the previous value.
  // https://arduinojson.org/v6/api/jsonvariant/to/
  template <typename T>
  typename enable_if<is_same<T, JsonObject>::value, JsonObject>::type to()
      const;

  // Sets the value to null.
  // ⚠️ Doesn't release the memory associated with the previous value.
  // https://arduinojson.org/v6/api/jsonvariant/to/
  template <typename T>
  typename enable_if<is_same<T, JsonVariant>::value, JsonVariant>::type to()
      const;

  // Returns true if the value is of the specified type.
  // https://arduinojson.org/v6/api/jsonvariant/is/
  template <typename T>
  FORCE_INLINE
      typename enable_if<ConverterNeedsWriteableRef<T>::value, bool>::type
      is() const {
    return Converter<T>::checkJson(getVariant());
  }

  // Returns true if the value is of the specified type.
  // https://arduinojson.org/v6/api/jsonvariant/is/
  template <typename T>
  FORCE_INLINE typename enable_if<!ConverterNeedsWriteableRef<T>::value &&
                                      !is_same<T, char*>::value &&
                                      !is_same<T, char>::value,
                                  bool>::type
  is() const {
    return Converter<T>::checkJson(getVariantConst());
  }

  // Shallow copies the specified value.
  // https://arduinojson.org/v6/api/jsonvariant/shallowcopy/
  FORCE_INLINE void shallowCopy(JsonVariantConst target) {
    VariantData* data = getOrCreateData();
    if (!data)
      return;
    const VariantData* targetData = VariantAttorney::getData(target);
    if (targetData)
      *data = *targetData;
    else
      data->setNull();
  }

  // Copies the specified value.
  // https://arduinojson.org/v6/api/jsonvariant/set/
  template <typename T>
  FORCE_INLINE bool set(const T& value) const {
    Converter<T>::toJson(value, getOrCreateVariant());
    MemoryPool* pool = getPool();
    return pool && !pool->overflowed();
  }

  // Copies the specified value.
  // https://arduinojson.org/v6/api/jsonvariant/set/
  template <typename T>
  FORCE_INLINE bool set(T* value) const {
    Converter<T*>::toJson(value, getOrCreateVariant());
    MemoryPool* pool = getPool();
    return pool && !pool->overflowed();
  }

  // Returns the size of the array or object.
  // https://arduinojson.org/v6/api/jsonvariant/size/
  FORCE_INLINE size_t size() const {
    return variantSize(getData());
  }

  // Returns the number of bytes occupied by the value.
  // https://arduinojson.org/v6/api/jsonvariant/memoryusage/
  FORCE_INLINE size_t memoryUsage() const {
    VariantData* data = getData();
    return data ? data->memoryUsage() : 0;
  }

  // Returns the depth (nesting level) of the value.
  // https://arduinojson.org/v6/api/jsonvariant/nesting/
  FORCE_INLINE size_t nesting() const {
    return variantNesting(getData());
  }

  // Appends a new (null) element to the array.
  // Returns a reference to the new element.
  // https://arduinojson.org/v6/api/jsonvariant/add/
  FORCE_INLINE JsonVariant add() const;

  // Appends a value to the array.
  // https://arduinojson.org/v6/api/jsonvariant/add/
  template <typename T>
  FORCE_INLINE bool add(const T& value) const {
    return add().set(value);
  }

  // Appends a value to the array.
  // https://arduinojson.org/v6/api/jsonvariant/add/
  template <typename T>
  FORCE_INLINE bool add(T* value) const {
    return add().set(value);
  }

  // Removes an element of the array.
  // ⚠️ Doesn't release the memory associated with the removed element.
  // https://arduinojson.org/v6/api/jsonvariant/remove/
  FORCE_INLINE void remove(size_t index) const {
    VariantData* data = getData();
    if (data)
      data->remove(index);
  }

  // Removes a member of the object.
  // ⚠️ Doesn't release the memory associated with the removed element.
  // https://arduinojson.org/v6/api/jsonvariant/remove/
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar*>::value>::type remove(
      TChar* key) const {
    VariantData* data = getData();
    if (data)
      data->remove(adaptString(key));
  }

  // Removes a member of the object.
  // ⚠️ Doesn't release the memory associated with the removed element.
  // https://arduinojson.org/v6/api/jsonvariant/remove/
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value>::type remove(
      const TString& key) const {
    VariantData* data = getData();
    if (data)
      data->remove(adaptString(key));
  }

  // Creates an array and appends it to the array.
  // https://arduinojson.org/v6/api/jsonvariant/createnestedarray/
  FORCE_INLINE JsonArray createNestedArray() const;

  // Creates an object and appends it to the array.
  // https://arduinojson.org/v6/api/jsonvariant/createnestedobject/
  FORCE_INLINE JsonObject createNestedObject() const;

  // Gets or sets an array element.
  // https://arduinojson.org/v6/api/jsonvariant/subscript/
  FORCE_INLINE ElementProxy<TDerived> operator[](size_t index) const;

  // Returns true if the object contains the specified key.
  // https://arduinojson.org/v6/api/jsonvariant/containskey/
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value, bool>::type
  containsKey(const TString& key) const;

  // Returns true if the object contains the specified key.
  // https://arduinojson.org/v6/api/jsonvariant/containskey/
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar*>::value, bool>::type
  containsKey(TChar* key) const;

  // Gets or sets an object member.
  // https://arduinojson.org/v6/api/jsonvariant/subscript/
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value,
                                  MemberProxy<TDerived, TString> >::type
  operator[](const TString& key) const;

  // Gets or sets an object member.
  // https://arduinojson.org/v6/api/jsonvariant/subscript/
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar*>::value,
                                  MemberProxy<TDerived, TChar*> >::type
  operator[](TChar* key) const;

  // Creates an array and adds it to the object.
  // https://arduinojson.org/v6/api/jsonvariant/createnestedarray/
  template <typename TString>
  FORCE_INLINE JsonArray createNestedArray(const TString& key) const;

  // Creates an array and adds it to the object.
  // https://arduinojson.org/v6/api/jsonvariant/createnestedarray/
  template <typename TChar>
  FORCE_INLINE JsonArray createNestedArray(TChar* key) const;

  // Creates an object and adds it to the object.
  // https://arduinojson.org/v6/api/jsonvariant/createnestedobject/
  template <typename TString>
  JsonObject createNestedObject(const TString& key) const;

  // Creates an object and adds it to the object.
  // https://arduinojson.org/v6/api/jsonvariant/createnestedobject/
  template <typename TChar>
  JsonObject createNestedObject(TChar* key) const;

 private:
  TDerived& derived() {
    return static_cast<TDerived&>(*this);
  }

  const TDerived& derived() const {
    return static_cast<const TDerived&>(*this);
  }

  FORCE_INLINE MemoryPool* getPool() const {
    return VariantAttorney::getPool(derived());
  }

  FORCE_INLINE VariantData* getData() const {
    return VariantAttorney::getData(derived());
  }

  FORCE_INLINE VariantData* getOrCreateData() const {
    return VariantAttorney::getOrCreateData(derived());
  }

 private:
  FORCE_INLINE JsonVariant getVariant() const;

  FORCE_INLINE JsonVariantConst getVariantConst() const {
    return JsonVariantConst(getData());
  }

  FORCE_INLINE JsonVariant getOrCreateVariant() const;
};

}  // namespace ARDUINOJSON_NAMESPACE
