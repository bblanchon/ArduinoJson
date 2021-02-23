// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <stddef.h>
#include <stdint.h>  // for uint8_t

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Misc/Visitable.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <ArduinoJson/Variant/VariantAs.hpp>
#include <ArduinoJson/Variant/VariantFunctions.hpp>
#include <ArduinoJson/Variant/VariantIs.hpp>
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
  template <typename T>
  FORCE_INLINE bool is() const {
    return variantIs<T>(_data);
  }

  FORCE_INLINE bool isNull() const {
    return variantIsNull(_data);
  }

  FORCE_INLINE bool isUndefined() const {
    return !_data;
  }

  FORCE_INLINE size_t memoryUsage() const {
    return _data ? _data->memoryUsage() : 0;
  }

  FORCE_INLINE size_t nesting() const {
    return _data ? _data->nesting() : 0;
  }

  size_t size() const {
    return variantSize(_data);
  }

 protected:
  VariantRefBase(TData *data) : _data(data) {}
  TData *_data;
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
                   public VariantShortcuts<VariantRef>,
                   public Visitable {
  typedef VariantRefBase<VariantData> base_type;
  friend class VariantConstRef;

 public:
  // Intenal use only
  FORCE_INLINE VariantRef(MemoryPool *pool, VariantData *data)
      : base_type(data), _pool(pool) {}

  // Creates an uninitialized VariantRef
  FORCE_INLINE VariantRef() : base_type(0), _pool(0) {}

  FORCE_INLINE void clear() const {
    return variantSetNull(_data);
  }

  // set(bool value)
  template <typename T>
  FORCE_INLINE bool set(
      T value, typename enable_if<is_same<T, bool>::value>::type * = 0) const {
    return variantSetBoolean(_data, value);
  }

  // set(double value);
  // set(float value);
  template <typename T>
  FORCE_INLINE bool set(
      T value,
      typename enable_if<is_floating_point<T>::value>::type * = 0) const {
    return variantSetFloat(_data, static_cast<Float>(value));
  }

  // set(char)
  // set(signed short)
  // set(signed int)
  // set(signed long)
  // set(signed char)
  // set(unsigned short)
  // set(unsigned int)
  // set(unsigned long)
  template <typename T>
  FORCE_INLINE bool set(
      T value,
      typename enable_if<is_integral<T>::value && !is_same<bool, T>::value &&
                         !is_same<char, T>::value>::type * = 0) const {
    return variantSetInteger<T>(_data, value);
  }

  // set(SerializedValue<const char *>)
  FORCE_INLINE bool set(SerializedValue<const char *> value) const {
    return variantSetLinkedRaw(_data, value);
  }

  // set(SerializedValue<std::string>)
  // set(SerializedValue<String>)
  // set(SerializedValue<const __FlashStringHelper*>)
  template <typename T>
  FORCE_INLINE bool set(
      SerializedValue<T> value,
      typename enable_if<!is_same<const char *, T>::value>::type * = 0) const {
    return variantSetOwnedRaw(_data, value, _pool);
  }

  // set(const std::string&)
  // set(const String&)
  template <typename T>
  FORCE_INLINE bool set(
      const T &value,
      typename enable_if<IsString<T>::value>::type * = 0) const {
    return variantSetString(_data, adaptString(value), _pool);
  }
  // set(char*)
  // set(const __FlashStringHelper*)
  // set(const char*)
  template <typename T>
  FORCE_INLINE bool set(
      T *value, typename enable_if<IsString<T *>::value>::type * = 0) const {
    return variantSetString(_data, adaptString(value), _pool);
  }

  // set(VariantRef)
  // set(VariantConstRef)
  // set(ArrayRef)
  // set(ArrayConstRef)
  // set(ObjectRef)
  // set(ObjecConstRef)
  // set(const JsonDocument&)
  template <typename TVariant>
  typename enable_if<IsVisitable<TVariant>::value, bool>::type set(
      const TVariant &value) const;

  // set(enum value)
  template <typename T>
  FORCE_INLINE bool set(
      T value, typename enable_if<is_enum<T>::value>::type * = 0) const {
    return variantSetInteger(_data, static_cast<Integer>(value));
  }

#if ARDUINOJSON_HAS_NULLPTR
  // set(nullptr_t)
  FORCE_INLINE bool set(decltype(nullptr)) const {
    variantSetNull(_data);
    return true;
  }
#endif

  template <typename T>
  FORCE_INLINE typename VariantAs<T>::type as() const {
    /********************************************************************
     **                THIS IS NOT A BUG IN THE LIBRARY                **
     **                --------------------------------                **
     **  Get a compilation error pointing here?                        **
     **  It doesn't mean the error *is* here.                          **
     **  Often, it's because you try to extract the wrong value type.  **
     **                                                                **
     **  For example:                                                  **
     **    char* name = doc["name"];                                   **
     **    char age = doc["age"];                                      **
     **  Instead, use:                                                 **
     **    const char* name = doc["name"];                             **
     **    int8_t age = doc["age"];                                    **
     ********************************************************************/
    return variantAs<typename VariantAs<T>::type>(_data, _pool);
  }

  template <typename T>
  FORCE_INLINE operator T() const {
    return variantAs<T>(_data, _pool);
  }

  template <typename TVisitor>
  typename TVisitor::result_type accept(TVisitor &visitor) const {
    return variantAccept(_data, visitor);
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

  VariantRef addElement() const;

  FORCE_INLINE VariantRef getElement(size_t) const;

  FORCE_INLINE VariantRef getOrAddElement(size_t) const;

  // getMember(const char*) const
  // getMember(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE VariantRef getMember(TChar *) const;

  // getMember(const std::string&) const
  // getMember(const String&) const
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value, VariantRef>::type
  getMember(const TString &) const;

  // getOrAddMember(char*) const
  // getOrAddMember(const char*) const
  // getOrAddMember(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE VariantRef getOrAddMember(TChar *) const;

  // getOrAddMember(const std::string&) const
  // getOrAddMember(const String&) const
  template <typename TString>
  FORCE_INLINE VariantRef getOrAddMember(const TString &) const;

  FORCE_INLINE void remove(size_t index) const {
    if (_data)
      _data->remove(index);
  }
  // remove(char*) const
  // remove(const char*) const
  // remove(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar *>::value>::type remove(
      TChar *key) const {
    if (_data)
      _data->remove(adaptString(key));
  }
  // remove(const std::string&) const
  // remove(const String&) const
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value>::type remove(
      const TString &key) const {
    if (_data)
      _data->remove(adaptString(key));
  }

 private:
  MemoryPool *_pool;
};  // namespace ARDUINOJSON_NAMESPACE

class VariantConstRef : public VariantRefBase<const VariantData>,
                        public VariantOperators<VariantConstRef>,
                        public VariantShortcuts<VariantConstRef>,
                        public Visitable {
  typedef VariantRefBase<const VariantData> base_type;
  friend class VariantRef;

 public:
  VariantConstRef() : base_type(0) {}
  VariantConstRef(const VariantData *data) : base_type(data) {}
  VariantConstRef(VariantRef var) : base_type(var._data) {}

  template <typename TVisitor>
  typename TVisitor::result_type accept(TVisitor &visitor) const {
    return variantAccept(_data, visitor);
  }

  template <typename T>
  FORCE_INLINE typename VariantConstAs<T>::type as() const {
    return variantAs<typename VariantConstAs<T>::type>(_data);
  }

  template <typename T>
  FORCE_INLINE operator T() const {
    return variantAs<T>(_data);
  }

  FORCE_INLINE VariantConstRef getElement(size_t) const;

  FORCE_INLINE VariantConstRef operator[](size_t index) const {
    return getElement(index);
  }

  // getMember(const std::string&) const
  // getMember(const String&) const
  template <typename TString>
  FORCE_INLINE VariantConstRef getMember(const TString &key) const {
    return VariantConstRef(
        objectGetMember(variantAsObject(_data), adaptString(key)));
  }

  // getMember(char*) const
  // getMember(const char*) const
  // getMember(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE VariantConstRef getMember(TChar *key) const {
    const CollectionData *obj = variantAsObject(_data);
    return VariantConstRef(obj ? obj->getMember(adaptString(key)) : 0);
  }

  // operator[](const std::string&) const
  // operator[](const String&) const
  template <typename TString>
  FORCE_INLINE
      typename enable_if<IsString<TString>::value, VariantConstRef>::type
      operator[](const TString &key) const {
    return getMember(key);
  }

  // operator[](char*) const
  // operator[](const char*) const
  // operator[](const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE
      typename enable_if<IsString<TChar *>::value, VariantConstRef>::type
      operator[](TChar *key) const {
    return getMember(key);
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
