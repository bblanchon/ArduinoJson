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

  template <typename T>
  using ConversionSupported =
      detail::is_same<typename detail::function_traits<
                          decltype(&Converter<T>::fromJson)>::arg1_type,
                      JsonVariantConst>;

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
  template <typename T,
            detail::enable_if_t<ConversionSupported<T>::value, bool> = true>
  T as() const {
    return Converter<T>::fromJson(*this);
  }

  // Invalid conversion. Will not compile.
  template <typename T,
            detail::enable_if_t<!ConversionSupported<T>::value, bool> = true>
  detail::InvalidConversion<JsonVariantConst, T> as() const;

  // Returns true if the value is of the specified type.
  // https://arduinojson.org/v7/api/jsonvariantconst/is/
  template <typename T>
  detail::enable_if_t<ConversionSupported<T>::value, bool> is() const {
    return Converter<T>::checkJson(*this);
  }

  // Always returns false for the unsupported types.
  // https://arduinojson.org/v7/api/jsonvariantconst/is/
  template <typename T>
  detail::enable_if_t<!ConversionSupported<T>::value, bool> is() const {
    return false;
  }

  template <typename T>
  operator T() const {
    return as<T>();
  }

  // Gets array's element at specified index.
  // https://arduinojson.org/v7/api/jsonvariantconst/subscript/
  template <typename T>
  detail::enable_if_t<detail::is_integral<T>::value, JsonVariantConst>
  operator[](T index) const {
    return JsonVariantConst(
        detail::VariantData::getElement(data_, size_t(index), resources_),
        resources_);
  }

  // Gets object's member with specified key.
  // https://arduinojson.org/v7/api/jsonvariantconst/subscript/
  template <typename TString>
  detail::enable_if_t<detail::IsString<TString>::value, JsonVariantConst>
  operator[](const TString& key) const {
    return JsonVariantConst(detail::VariantData::getMember(
                                data_, detail::adaptString(key), resources_),
                            resources_);
  }

  // Gets object's member with specified key.
  // https://arduinojson.org/v7/api/jsonvariantconst/subscript/
  template <typename TChar>
  detail::enable_if_t<detail::IsString<TChar*>::value, JsonVariantConst>
  operator[](TChar* key) const {
    return JsonVariantConst(detail::VariantData::getMember(
                                data_, detail::adaptString(key), resources_),
                            resources_);
  }

  // Gets object's member with specified key or the array's element at the
  // specified index.
  // https://arduinojson.org/v7/api/jsonvariantconst/subscript/
  template <typename TVariant>
  detail::enable_if_t<detail::IsVariant<TVariant>::value, JsonVariantConst>
  operator[](const TVariant& key) const {
    if (key.template is<size_t>())
      return operator[](key.template as<size_t>());
    else
      return operator[](key.template as<const char*>());
  }

  // DEPRECATED: use obj[key].is<T>() instead
  // https://arduinojson.org/v7/api/jsonvariantconst/containskey/
  template <typename TString>
  ARDUINOJSON_DEPRECATED("use var[key].is<T>() instead")
  detail::enable_if_t<detail::IsString<TString>::value, bool> containsKey(
      const TString& key) const {
    return detail::VariantData::getMember(getData(), detail::adaptString(key),
                                          resources_) != 0;
  }

  // DEPRECATED: use obj["key"].is<T>() instead
  // https://arduinojson.org/v7/api/jsonvariantconst/containskey/
  template <typename TChar>
  ARDUINOJSON_DEPRECATED("use obj[\"key\"].is<T>() instead")
  detail::enable_if_t<detail::IsString<TChar*>::value, bool> containsKey(
      TChar* key) const {
    return detail::VariantData::getMember(getData(), detail::adaptString(key),
                                          resources_) != 0;
  }

  // DEPRECATED: use obj[key].is<T>() instead
  // https://arduinojson.org/v7/api/jsonvariantconst/containskey/
  template <typename TVariant>
  ARDUINOJSON_DEPRECATED("use var[key].is<T>() instead")
  detail::enable_if_t<detail::IsVariant<TVariant>::value, bool> containsKey(
      const TVariant& key) const {
    return containsKey(key.template as<const char*>());
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
