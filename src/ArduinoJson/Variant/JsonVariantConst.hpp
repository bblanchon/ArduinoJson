// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
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

namespace ARDUINOJSON_NAMESPACE {

// Forward declarations.
class JsonArray;
class JsonObject;

// A read-only reference to a value in a JsonDocument
// https://arduinojson.org/v6/api/jsonarrayconst/
class JsonVariantConst : public VariantTag,
                         public VariantOperators<JsonVariantConst> {
  friend class VariantAttorney;

 public:
  // Creates an unbound reference.
  JsonVariantConst() : _data(0) {}

  // INTERNAL USE ONLY
  explicit JsonVariantConst(const VariantData* data) : _data(data) {}

  // Returns true if the value is null or the reference is unbound.
  // https://arduinojson.org/v6/api/jsonvariantconst/isnull/
  FORCE_INLINE bool isNull() const {
    return variantIsNull(_data);
  }

  // Returns true if the reference is unbound.
  FORCE_INLINE bool isUnbound() const {
    return !_data;
  }

  // Returns the number of bytes occupied by the value.
  // https://arduinojson.org/v6/api/jsonvariantconst/memoryusage/
  FORCE_INLINE size_t memoryUsage() const {
    return _data ? _data->memoryUsage() : 0;
  }

  // Returns the depth (nesting level) of the value.
  // https://arduinojson.org/v6/api/jsonvariantconst/nesting/
  FORCE_INLINE size_t nesting() const {
    return variantNesting(_data);
  }

  // Returns the size of the array or object.
  // https://arduinojson.org/v6/api/jsonvariantconst/size/
  size_t size() const {
    return variantSize(_data);
  }

  // Casts the value to the specified type.
  // https://arduinojson.org/v6/api/jsonvariantconst/as/
  template <typename T>
  FORCE_INLINE
      typename enable_if<!is_same<T, char*>::value && !is_same<T, char>::value,
                         T>::type
      as() const {
    return Converter<T>::fromJson(*this);
  }

  // Returns true if the value is of the specified type.
  // https://arduinojson.org/v6/api/jsonvariantconst/is/
  template <typename T>
  FORCE_INLINE
      typename enable_if<!is_same<T, char*>::value && !is_same<T, char>::value,
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
    return JsonVariantConst(variantGetElement(_data, index));
  }

  // Gets object's member with specified key.
  // https://arduinojson.org/v6/api/jsonvariantconst/subscript/
  template <typename TString>
  FORCE_INLINE
      typename enable_if<IsString<TString>::value, JsonVariantConst>::type
      operator[](const TString& key) const {
    return JsonVariantConst(variantGetMember(_data, adaptString(key)));
  }

  // Gets object's member with specified key.
  // https://arduinojson.org/v6/api/jsonvariantconst/subscript/
  template <typename TChar>
  FORCE_INLINE
      typename enable_if<IsString<TChar*>::value, JsonVariantConst>::type
      operator[](TChar* key) const {
    return JsonVariantConst(variantGetMember(_data, adaptString(key)));
  }

  // Returns true if tge object contains the specified key.
  // https://arduinojson.org/v6/api/jsonvariantconst/containskey/
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value, bool>::type
  containsKey(const TString& key) const {
    return variantGetMember(getData(), adaptString(key)) != 0;
  }

  // Returns true if tge object contains the specified key.
  // https://arduinojson.org/v6/api/jsonvariantconst/containskey/
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar*>::value, bool>::type
  containsKey(TChar* key) const {
    return variantGetMember(getData(), adaptString(key)) != 0;
  }

 protected:
  const VariantData* getData() const {
    return _data;
  }

 private:
  const VariantData* _data;
};

}  // namespace ARDUINOJSON_NAMESPACE
