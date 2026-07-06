// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
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
  JsonVariantConst() {}

  // INTERNAL USE ONLY
  explicit JsonVariantConst(detail::VariantData* data,
                            detail::ResourceManager* resources)
      : impl_(data, resources) {}

  // Returns true if the value is null or the reference is unbound.
  // https://arduinojson.org/v7/api/jsonvariantconst/isnull/
  bool isNull() const {
    return impl_.isNull();
  }

  // Returns true if the reference is unbound.
  bool isUnbound() const {
    return impl_.data() == nullptr;
  }

  // Returns the depth (nesting level) of the value.
  // https://arduinojson.org/v7/api/jsonvariantconst/nesting/
  size_t nesting() const {
    return impl_.nesting();
  }

  // Returns the size of the array or object.
  // https://arduinojson.org/v7/api/jsonvariantconst/size/
  size_t size() const {
    return impl_.size();
  }

  // Casts the value to the specified type.
  // https://arduinojson.org/v7/api/jsonvariantconst/as/
  template <typename T,
            detail::enable_if_t<ConversionSupported<T>::value, int> = 0>
  T as() const {
    return Converter<T>::fromJson(*this);
  }

  // Invalid conversion. Will not compile.
  template <typename T,
            detail::enable_if_t<!ConversionSupported<T>::value, int> = 0>
  detail::InvalidConversion<JsonVariantConst, T> as() const;

  // Returns true if the value is of the specified type.
  // https://arduinojson.org/v7/api/jsonvariantconst/is/
  template <typename T,
            detail::enable_if_t<ConversionSupported<T>::value, int> = 0>
  bool is() const {
    return Converter<T>::checkJson(*this);
  }

  // Always returns false for the unsupported types.
  // https://arduinojson.org/v7/api/jsonvariantconst/is/
  template <typename T,
            detail::enable_if_t<!ConversionSupported<T>::value, int> = 0>
  bool is() const {
    return false;
  }

  template <typename T>
  operator T() const {
    return as<T>();
  }

  // Gets array's element at specified index.
  // https://arduinojson.org/v7/api/jsonvariantconst/subscript/
  template <typename T,
            detail::enable_if_t<detail::is_integral<T>::value, int> = 0>
  JsonVariantConst operator[](T index) const {
    return JsonVariantConst(impl_.getElement(size_t(index)), impl_.resources());
  }

  // Gets object's member with specified key.
  // https://arduinojson.org/v7/api/jsonvariantconst/subscript/
  template <typename TString,
            detail::enable_if_t<detail::IsString<TString>::value, int> = 0>
  JsonVariantConst operator[](const TString& key) const {
    return JsonVariantConst(impl_.getMember(detail::adaptString(key)),
                            impl_.resources());
  }

  // Gets object's member with specified key.
  // https://arduinojson.org/v7/api/jsonvariantconst/subscript/
  template <typename TChar,
            detail::enable_if_t<detail::IsString<TChar*>::value, int> = 0>
  JsonVariantConst operator[](TChar* key) const {
    return JsonVariantConst(impl_.getMember(detail::adaptString(key)),
                            impl_.resources());
  }

  // Gets object's member with specified key or the array's element at the
  // specified index.
  // https://arduinojson.org/v7/api/jsonvariantconst/subscript/
  template <typename TVariant,
            detail::enable_if_t<detail::IsVariant<TVariant>::value, int> = 0>
  JsonVariantConst operator[](const TVariant& key) const {
    if (key.template is<size_t>())
      return operator[](key.template as<size_t>());
    else
      return operator[](key.template as<JsonString>());
  }

  // DEPRECATED: use obj[key].is<T>() instead
  // https://arduinojson.org/v7/api/jsonvariantconst/containskey/
  template <typename TString,
            detail::enable_if_t<detail::IsString<TString>::value, int> = 0>
  ARDUINOJSON_DEPRECATED("use var[key].is<T>() instead")
  bool containsKey(const TString& key) const {
    return impl_.getMember(detail::adaptString(key)) != 0;
  }

  // DEPRECATED: use obj["key"].is<T>() instead
  // https://arduinojson.org/v7/api/jsonvariantconst/containskey/
  template <typename TChar,
            detail::enable_if_t<detail::IsString<TChar*>::value, int> = 0>
  ARDUINOJSON_DEPRECATED("use obj[\"key\"].is<T>() instead")
  bool containsKey(TChar* key) const {
    return impl_.getMember(detail::adaptString(key)) != 0;
  }

  // DEPRECATED: use obj[key].is<T>() instead
  // https://arduinojson.org/v7/api/jsonvariantconst/containskey/
  template <typename TVariant,
            detail::enable_if_t<detail::IsVariant<TVariant>::value, int> = 0>
  ARDUINOJSON_DEPRECATED("use var[key].is<T>() instead")
  bool containsKey(const TVariant& key) const {
    return containsKey(key.template as<const char*>());
  }

  // DEPRECATED: always returns zero
  ARDUINOJSON_DEPRECATED("always returns zero")
  size_t memoryUsage() const {
    return 0;
  }

 protected:
  detail::VariantData* getData() const {
    return impl_.data();
  }

  detail::ResourceManager* getResourceManager() const {
    return impl_.resources();
  }

 private:
  mutable detail::VariantImpl impl_;
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
