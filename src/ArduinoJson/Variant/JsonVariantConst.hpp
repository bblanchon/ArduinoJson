// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <stddef.h>
#include <stdint.h>  // for uint8_t

#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Strings/IsString.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <ArduinoJson/Variant/VariantAttorney.hpp>
#include <ArduinoJson/Variant/VariantOperators.hpp>
#include <ArduinoJson/Variant/VariantTag.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// Forward declarations.
class JsonArray;
class JsonObject;

// A read-only reference to a value in a JsonDocument
// https://arduinojson.org/v7/api/jsonarrayconst/
class JsonVariantConst : public detail::VariantTag,
                         public detail::VariantOperators<JsonVariantConst> {
  friend class detail::VariantAttorney;

 public:
  // Creates an unbound reference.
  JsonVariantConst() : data_(nullptr), resources_(nullptr) {}

  // INTERNAL USE ONLY
  explicit JsonVariantConst(const detail::VariantData* data,
                            const detail::ResourceManager* resources)
      : data_(data), resources_(resources) {}

  // Returns true if the value is null or the reference is unbound.
  // https://arduinojson.org/v7/api/jsonvariantconst/isnull/
  bool isNull() const {
    return detail::VariantData::isNull(data_);
  }

  // Returns true if the reference is unbound.
  bool isUnbound() const {
    return !data_;
  }

  // Returns the depth (nesting level) of the value.
  // https://arduinojson.org/v7/api/jsonvariantconst/nesting/
  size_t nesting() const {
    return detail::VariantData::nesting(data_, resources_);
  }

  // Returns the size of the array or object.
  // https://arduinojson.org/v7/api/jsonvariantconst/size/
  size_t size() const {
    return detail::VariantData::size(data_, resources_);
  }

  // Casts the value to the specified type.
  // https://arduinojson.org/v7/api/jsonvariantconst/as/
  template <typename T>
  typename detail::enable_if<!detail::is_same<T, char*>::value &&
                                 !detail::is_same<T, char>::value,
                             T>::type
  as() const {
    return Converter<T>::fromJson(*this);
  }

  // Returns true if the value is of the specified type.
  // https://arduinojson.org/v7/api/jsonvariantconst/is/
  template <typename T>
  typename detail::enable_if<!detail::is_same<T, char*>::value &&
                                 !detail::is_same<T, char>::value,
                             bool>::type
  is() const {
    return Converter<T>::checkJson(*this);
  }

  template <typename T>
  operator T() const {
    return as<T>();
  }

  // Gets array's element at specified index.
  // https://arduinojson.org/v7/api/jsonvariantconst/subscript/
  JsonVariantConst operator[](size_t index) const {
    return JsonVariantConst(
        detail::VariantData::getElement(data_, index, resources_), resources_);
  }

  // Gets object's member with specified key.
  // https://arduinojson.org/v7/api/jsonvariantconst/subscript/
  template <typename TString>
  typename detail::enable_if<detail::IsString<TString>::value,
                             JsonVariantConst>::type
  operator[](const TString& key) const {
    return JsonVariantConst(detail::VariantData::getMember(
                                data_, detail::adaptString(key), resources_),
                            resources_);
  }

  // Gets object's member with specified key.
  // https://arduinojson.org/v7/api/jsonvariantconst/subscript/
  template <typename TChar>
  typename detail::enable_if<detail::IsString<TChar*>::value,
                             JsonVariantConst>::type
  operator[](TChar* key) const {
    return JsonVariantConst(detail::VariantData::getMember(
                                data_, detail::adaptString(key), resources_),
                            resources_);
  }

  // Returns true if tge object contains the specified key.
  // https://arduinojson.org/v7/api/jsonvariantconst/containskey/
  template <typename TString>

  typename detail::enable_if<detail::IsString<TString>::value, bool>::type
  containsKey(const TString& key) const {
    return detail::VariantData::getMember(getData(), detail::adaptString(key),
                                          resources_) != 0;
  }

  // Returns true if tge object contains the specified key.
  // https://arduinojson.org/v7/api/jsonvariantconst/containskey/
  template <typename TChar>

  typename detail::enable_if<detail::IsString<TChar*>::value, bool>::type
  containsKey(TChar* key) const {
    return detail::VariantData::getMember(getData(), detail::adaptString(key),
                                          resources_) != 0;
  }

  // DEPRECATED: always returns zero
  ARDUINOJSON_DEPRECATED("always returns zero")
  size_t memoryUsage() const {
    return 0;
  }

 protected:
  const detail::VariantData* getData() const {
    return data_;
  }

  const detail::ResourceManager* getResourceManager() const {
    return resources_;
  }

 private:
  const detail::VariantData* data_;
  const detail::ResourceManager* resources_;
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
