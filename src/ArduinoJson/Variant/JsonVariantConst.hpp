// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <stddef.h>
#include <stdint.h>  // for uint8_t

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Strings/IsString.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <ArduinoJson/Variant/VariantAttorney.hpp>
#include <ArduinoJson/Variant/VariantFunctions.hpp>
#include <ArduinoJson/Variant/VariantOperators.hpp>
#include <ArduinoJson/Variant/VariantTag.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// Forward declarations.
class JsonArray;
class JsonObject;

// A read-only reference to a value in a JsonDocument
// https://arduinojson.org/v6/api/jsonarrayconst/
class JsonVariantConst : public detail::VariantTag,
                         public detail::VariantOperators<JsonVariantConst> {
  friend class detail::VariantAttorney;

 public:
  // Creates an unbound reference.
  JsonVariantConst() : data_(0) {}

  // INTERNAL USE ONLY
  explicit JsonVariantConst(const detail::VariantData* data) : data_(data) {}

  // Returns true if the value is null or the reference is unbound.
  // https://arduinojson.org/v6/api/jsonvariantconst/isnull/
  FORCE_INLINE bool isNull() const {
    using namespace detail;
    return variantIsNull(data_);
  }

  // Returns true if the reference is unbound.
  FORCE_INLINE bool isUnbound() const {
    return !data_;
  }

  // Returns the number of bytes occupied by the value.
  // https://arduinojson.org/v6/api/jsonvariantconst/memoryusage/
  FORCE_INLINE size_t memoryUsage() const {
    return data_ ? data_->memoryUsage() : 0;
  }

  // Returns the depth (nesting level) of the value.
  // https://arduinojson.org/v6/api/jsonvariantconst/nesting/
  FORCE_INLINE size_t nesting() const {
    return variantNesting(data_);
  }

  // Returns the size of the array or object.
  // https://arduinojson.org/v6/api/jsonvariantconst/size/
  size_t size() const {
    return variantSize(data_);
  }

  // Casts the value to the specified type.
  // https://arduinojson.org/v6/api/jsonvariantconst/as/
  template <typename T>
  FORCE_INLINE typename detail::enable_if<!detail::is_same<T, char*>::value &&
                                              !detail::is_same<T, char>::value,
                                          T>::type
  as() const {
    return Converter<T>::fromJson(*this);
  }

  // Returns true if the value is of the specified type.
  // https://arduinojson.org/v6/api/jsonvariantconst/is/
  template <typename T>
  FORCE_INLINE typename detail::enable_if<!detail::is_same<T, char*>::value &&
                                              !detail::is_same<T, char>::value,
                                          bool>::type
  is() const {
    return Converter<T>::checkJson(*this);
  }

  template <typename T>
  FORCE_INLINE operator T() const {
    return as<T>();
  }

  // Gets array's element at specified index.
  // https://arduinojson.org/v6/api/jsonvariantconst/subscript/
  FORCE_INLINE JsonVariantConst operator[](size_t index) const {
    return JsonVariantConst(variantGetElement(data_, index));
  }

  // Gets object's member with specified key.
  // https://arduinojson.org/v6/api/jsonvariantconst/subscript/
  template <typename TString>
  FORCE_INLINE typename detail::enable_if<detail::IsString<TString>::value,
                                          JsonVariantConst>::type
  operator[](const TString& key) const {
    return JsonVariantConst(variantGetMember(data_, detail::adaptString(key)));
  }

  // Gets object's member with specified key.
  // https://arduinojson.org/v6/api/jsonvariantconst/subscript/
  template <typename TChar>
  FORCE_INLINE typename detail::enable_if<detail::IsString<TChar*>::value,
                                          JsonVariantConst>::type
  operator[](TChar* key) const {
    return JsonVariantConst(variantGetMember(data_, detail::adaptString(key)));
  }

  // Returns true if tge object contains the specified key.
  // https://arduinojson.org/v6/api/jsonvariantconst/containskey/
  template <typename TString>
  FORCE_INLINE
      typename detail::enable_if<detail::IsString<TString>::value, bool>::type
      containsKey(const TString& key) const {
    return variantGetMember(getData(), detail::adaptString(key)) != 0;
  }

  // Returns true if tge object contains the specified key.
  // https://arduinojson.org/v6/api/jsonvariantconst/containskey/
  template <typename TChar>
  FORCE_INLINE
      typename detail::enable_if<detail::IsString<TChar*>::value, bool>::type
      containsKey(TChar* key) const {
    return variantGetMember(getData(), detail::adaptString(key)) != 0;
  }

 protected:
  const detail::VariantData* getData() const {
    return data_;
  }

 private:
  const detail::VariantData* data_;
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
