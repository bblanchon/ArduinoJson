// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Variant/Converter.hpp>
#include <ArduinoJson/Variant/VariantConstRef.hpp>
#include <ArduinoJson/Variant/VariantOperators.hpp>
#include <ArduinoJson/Variant/VariantShortcuts.hpp>
#include <ArduinoJson/Variant/VariantTo.hpp>

namespace ARDUINOJSON_NAMESPACE {

class VariantRef;

template <typename TDataSource>
class VariantRefBase : public VariantShortcuts<VariantRefBase<TDataSource> >,
                       public VariantTag {
  friend class VariantAttorney;

 public:
  explicit FORCE_INLINE VariantRefBase(TDataSource source) : _source(source) {}

  FORCE_INLINE void clear() const {
    variantSetNull(getData());
  }

  FORCE_INLINE bool isNull() const {
    return variantIsNull(getData());
  }

  FORCE_INLINE bool isUnbound() const {
    return !getData();
  }

  template <typename T>
  FORCE_INLINE typename enable_if<!is_same<T, char*>::value &&
                                      !is_same<T, char>::value &&
                                      !ConverterNeedsWriteableRef<T>::value,
                                  T>::type
  as() const {
    return Converter<T>::fromJson(getVariantConst());
  }

  template <typename T>
  FORCE_INLINE typename enable_if<ConverterNeedsWriteableRef<T>::value, T>::type
  as() const {
    return Converter<T>::fromJson(getVariant());
  }

  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, char*>::value, const char*>::type
  ARDUINOJSON_DEPRECATED("Replace as<char*>() with as<const char*>()")
      as() const {
    return as<const char*>();
  }

  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, char>::value, char>::type
  ARDUINOJSON_DEPRECATED(
      "Support for char is deprecated, use int8_t or uint8_t instead")
      as() const {
    return static_cast<char>(as<signed char>());
  }

  template <typename T>
  FORCE_INLINE operator T() const {
    return as<T>();
  }

  // Change the type of the variant
  //
  // ArrayRef to<ArrayRef>()
  template <typename T>
  typename enable_if<is_same<T, ArrayRef>::value, ArrayRef>::type to() const;
  //
  // ObjectRef to<ObjectRef>()
  template <typename T>
  typename enable_if<is_same<T, ObjectRef>::value, ObjectRef>::type to() const;
  //
  // VariantRef to<VariantRef>()
  template <typename T>
  typename enable_if<is_same<T, VariantRef>::value, VariantRef>::type to()
      const;

  template <typename T>
  FORCE_INLINE
      typename enable_if<ConverterNeedsWriteableRef<T>::value, bool>::type
      is() const {
    return Converter<T>::checkJson(getVariant());
  }

  template <typename T>
  FORCE_INLINE typename enable_if<!ConverterNeedsWriteableRef<T>::value &&
                                      !is_same<T, char*>::value &&
                                      !is_same<T, char>::value,
                                  bool>::type
  is() const {
    return Converter<T>::checkJson(getVariantConst());
  }

  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, char*>::value, bool>::type
  ARDUINOJSON_DEPRECATED("Replace is<char*>() with is<const char*>()")
      is() const {
    return is<const char*>();
  }

  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, char>::value, bool>::type
  ARDUINOJSON_DEPRECATED(
      "Support for char is deprecated, use int8_t or uint8_t instead")
      is() const {
    return is<signed char>();
  }

  FORCE_INLINE void shallowCopy(VariantConstRef target) {
    VariantData* data = getOrCreateData();
    if (!data)
      return;
    const VariantData* targetData = VariantAttorney::getData(target);
    if (targetData)
      *data = *targetData;
    else
      data->setNull();
  }

  template <typename T>
  FORCE_INLINE bool set(const T& value) const {
    Converter<T>::toJson(value, getOrCreateVariant());
    MemoryPool* pool = getPool();
    return pool && !pool->overflowed();
  }

  template <typename T>
  FORCE_INLINE bool set(T* value) const {
    Converter<T*>::toJson(value, getOrCreateVariant());
    MemoryPool* pool = getPool();
    return pool && !pool->overflowed();
  }

  bool ARDUINOJSON_DEPRECATED(
      "Support for char is deprecated, use int8_t or uint8_t instead")
      set(char value) const;

  FORCE_INLINE size_t size() const {
    return variantSize(getData());
  }

  FORCE_INLINE size_t memoryUsage() const {
    VariantData* data = getData();
    return data ? data->memoryUsage() : 0;
  }

  FORCE_INLINE size_t nesting() const {
    return variantNesting(getData());
  }

  FORCE_INLINE VariantRef add() const;

  using ArrayShortcuts<VariantRefBase<TDataSource> >::add;

  FORCE_INLINE void remove(size_t index) const {
    VariantData* data = getData();
    if (data)
      data->remove(index);
  }
  // remove(char*) const
  // remove(const char*) const
  // remove(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar*>::value>::type remove(
      TChar* key) const {
    VariantData* data = getData();
    if (data)
      data->remove(adaptString(key));
  }
  // remove(const std::string&) const
  // remove(const String&) const
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value>::type remove(
      const TString& key) const {
    VariantData* data = getData();
    if (data)
      data->remove(adaptString(key));
  }

 protected:
  FORCE_INLINE MemoryPool* getPool() const {
    return _source.getPool();
  }

  FORCE_INLINE VariantData* getData() const {
    return _source.getData();
  }

  FORCE_INLINE VariantData* getOrCreateData() const {
    return _source.getOrCreateData();
  }

 private:
  FORCE_INLINE VariantRef getVariant() const;

  FORCE_INLINE VariantConstRef getVariantConst() const {
    return VariantConstRef(getData());
  }

  FORCE_INLINE VariantRef getOrCreateVariant() const;

  TDataSource _source;
};

}  // namespace ARDUINOJSON_NAMESPACE
