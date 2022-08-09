// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <stddef.h>
#include <stdint.h>  // for uint8_t

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <ArduinoJson/Variant/Converter.hpp>
#include <ArduinoJson/Variant/VariantAttorney.hpp>
#include <ArduinoJson/Variant/VariantFunctions.hpp>
#include <ArduinoJson/Variant/VariantOperators.hpp>
#include <ArduinoJson/Variant/VariantRef.hpp>
#include <ArduinoJson/Variant/VariantShortcuts.hpp>
#include <ArduinoJson/Variant/VariantTag.hpp>

namespace ARDUINOJSON_NAMESPACE {

// Forward declarations.
class ArrayRef;
class ObjectRef;

// Contains the methods shared by VariantRef and VariantConstRef
template <typename TData>
class VariantRefBase : public VariantTag {
 public:
  FORCE_INLINE bool isNull() const {
    return variantIsNull(_data);
  }

  FORCE_INLINE bool isUnbound() const {
    return !_data;
  }

  FORCE_INLINE size_t memoryUsage() const {
    return _data ? _data->memoryUsage() : 0;
  }

  FORCE_INLINE size_t nesting() const {
    return variantNesting(_data);
  }

  size_t size() const {
    return variantSize(_data);
  }

 protected:
  VariantRefBase(TData* data) : _data(data) {}
  TData* _data;
};

class VariantConstRef : public VariantRefBase<const VariantData>,
                        public VariantOperators<VariantConstRef>,
                        public VariantShortcuts<VariantConstRef> {
  typedef VariantRefBase<const VariantData> base_type;

  friend class VariantAttorney;

 public:
  VariantConstRef() : base_type(0) {}
  explicit VariantConstRef(const VariantData* data) : base_type(data) {}

  template <typename T>
  FORCE_INLINE
      typename enable_if<!is_same<T, char*>::value && !is_same<T, char>::value,
                         T>::type
      as() const {
    return Converter<T>::fromJson(*this);
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
  FORCE_INLINE
      typename enable_if<!is_same<T, char*>::value && !is_same<T, char>::value,
                         bool>::type
      is() const {
    return Converter<T>::checkJson(*this);
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

  template <typename T>
  FORCE_INLINE operator T() const {
    return as<T>();
  }

  FORCE_INLINE VariantConstRef operator[](size_t index) const {
    return VariantConstRef(variantGetElement(_data, index));
  }

  // operator[](const std::string&) const
  // operator[](const String&) const
  template <typename TString>
  FORCE_INLINE
      typename enable_if<IsString<TString>::value, VariantConstRef>::type
      operator[](const TString& key) const {
    return VariantConstRef(variantGetMember(_data, adaptString(key)));
  }

  // operator[](char*) const
  // operator[](const char*) const
  // operator[](const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE
      typename enable_if<IsString<TChar*>::value, VariantConstRef>::type
      operator[](TChar* key) const {
    return VariantConstRef(variantGetMember(_data, adaptString(key)));
  }

 protected:
  const VariantData* getData() const {
    return _data;
  }
};

// A variant that can be a any value serializable to a JSON value.
//
// It can be set to:
// - a boolean
// - a char, short, int or a long (signed or unsigned)
// - a string (const char*)
// - a reference to a ArrayRef or ObjectRef
class VariantRef : public VariantRefBase<VariantData>,
                   public VariantOperators<VariantRef>,
                   public VariantShortcuts<VariantRef> {
  typedef VariantRefBase<VariantData> base_type;

 public:
  // Intenal use only
  FORCE_INLINE VariantRef(MemoryPool* pool, VariantData* data)
      : base_type(data), _pool(pool) {}

  // Creates an uninitialized VariantRef
  FORCE_INLINE VariantRef() : base_type(0), _pool(0) {}

  FORCE_INLINE void clear() const {
    return variantSetNull(_data);
  }

  template <typename T>
  FORCE_INLINE bool set(const T& value) const {
    Converter<T>::toJson(value, *this);
    return _pool && !_pool->overflowed();
  }

  bool ARDUINOJSON_DEPRECATED(
      "Support for char is deprecated, use int8_t or uint8_t instead")
      set(char value) const;

  template <typename T>
  FORCE_INLINE bool set(T* value) const {
    Converter<T*>::toJson(value, *this);
    return _pool && !_pool->overflowed();
  }

  template <typename T>
  FORCE_INLINE
      typename enable_if<!is_same<T, char*>::value && !is_same<T, char>::value,
                         T>::type
      as() const {
    return Converter<T>::fromJson(*this);
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
  FORCE_INLINE
      typename enable_if<!is_same<T, char*>::value && !is_same<T, char>::value,
                         bool>::type
      is() const {
    return Converter<T>::checkJson(*this);
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

  template <typename T>
  FORCE_INLINE operator T() const {
    return as<T>();
  }

  FORCE_INLINE operator VariantConstRef() const {
    return VariantConstRef(_data);
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
  // ObjectRef to<VariantRef>()
  template <typename T>
  typename enable_if<is_same<T, VariantRef>::value, VariantRef>::type to()
      const;

  VariantRef add() const {
    return VariantRef(_pool, variantAddElement(_data, _pool));
  }

  using ArrayShortcuts<VariantRef>::add;

  FORCE_INLINE void remove(size_t index) const {
    if (_data)
      _data->remove(index);
  }
  // remove(char*) const
  // remove(const char*) const
  // remove(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar*>::value>::type remove(
      TChar* key) const {
    if (_data)
      _data->remove(adaptString(key));
  }
  // remove(const std::string&) const
  // remove(const String&) const
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value>::type remove(
      const TString& key) const {
    if (_data)
      _data->remove(adaptString(key));
  }

  inline void shallowCopy(VariantConstRef target) {
    if (!_data)
      return;
    const VariantData* targetData = VariantAttorney::getData(target);
    if (targetData)
      *_data = *targetData;
    else
      _data->setNull();
  }

  MemoryPool* getPool() const {
    return _pool;
  }

  VariantData* getData() const {
    return _data;
  }

  VariantData* getOrCreateData() const {
    return _data;
  }

 private:
  MemoryPool* _pool;
};

template <>
struct Converter<VariantRef> : private VariantAttorney {
  static void toJson(VariantRef src, VariantRef dst) {
    variantCopyFrom(getData(dst), getData(src), getPool(dst));
  }

  static VariantRef fromJson(VariantRef src) {
    return src;
  }

  static InvalidConversion<VariantConstRef, VariantRef> fromJson(
      VariantConstRef);

  static bool checkJson(VariantRef src) {
    VariantData* data = getData(src);
    return !!data;
  }

  static bool checkJson(VariantConstRef) {
    return false;
  }
};

template <>
struct Converter<VariantConstRef> : private VariantAttorney {
  static void toJson(VariantConstRef src, VariantRef dst) {
    variantCopyFrom(getData(dst), getData(src), getPool(dst));
  }

  static VariantConstRef fromJson(VariantConstRef src) {
    return VariantConstRef(getData(src));
  }

  static bool checkJson(VariantConstRef src) {
    const VariantData* data = getData(src);
    return !!data;
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
