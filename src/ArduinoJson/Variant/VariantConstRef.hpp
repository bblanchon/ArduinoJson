// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <stddef.h>
#include <stdint.h>  // for uint8_t

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <ArduinoJson/Variant/VariantAttorney.hpp>
#include <ArduinoJson/Variant/VariantConstRef.hpp>
#include <ArduinoJson/Variant/VariantFunctions.hpp>
#include <ArduinoJson/Variant/VariantOperators.hpp>
#include <ArduinoJson/Variant/VariantShortcuts.hpp>
#include <ArduinoJson/Variant/VariantTag.hpp>

namespace ARDUINOJSON_NAMESPACE {

// Forward declarations.
class ArrayRef;
class ObjectRef;

class VariantConstRef : public VariantTag,
                        public VariantOperators<VariantConstRef>,
                        public VariantShortcuts<VariantConstRef> {
  friend class VariantAttorney;

 public:
  VariantConstRef() : _data(0) {}
  explicit VariantConstRef(const VariantData* data) : _data(data) {}

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

 private:
  const VariantData* _data;
};

}  // namespace ARDUINOJSON_NAMESPACE
