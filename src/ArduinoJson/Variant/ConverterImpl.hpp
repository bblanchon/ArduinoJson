// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Strings/IsWriteableString.hpp>
#include <ArduinoJson/Variant/VariantFunctions.hpp>
#include <ArduinoJson/Variant/VariantRef.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename T, typename Enable>
struct Converter {
  static bool toJson(VariantRef variant, const T& value) {
    // clang-format off
    return convertToJson(variant, value); // Error here? See https://arduinojson.org/v6/unsupported-set/
    // clang-format on
  }

  static T fromJson(VariantConstRef variant) {
    // clang-format off
    T value; // Error here? See https://arduinojson.org/v6/non-default-constructible/
    convertFromJson(value, variant);  // Error here? See https://arduinojson.org/v6/unsupported-as/
    // clang-format on
    return value;
  }

  static bool checkJson(VariantConstRef variant) {
    T dummy;
    // clang-format off
    return canConvertFromJson(dummy, variant);  // Error here? See https://arduinojson.org/v6/unsupported-is/
    // clang-format on
  }
};

template <typename T>
struct Converter<
    T, typename enable_if<is_integral<T>::value && !is_same<bool, T>::value &&
                          !is_same<char, T>::value>::type> {
  static bool toJson(VariantRef variant, T value) {
    VariantData* data = getData(variant);
    ARDUINOJSON_ASSERT_INTEGER_TYPE_IS_SUPPORTED(T);
    if (!data)
      return false;
    data->setInteger(value);
    return true;
  }

  static T fromJson(VariantConstRef variant) {
    ARDUINOJSON_ASSERT_INTEGER_TYPE_IS_SUPPORTED(T);
    const VariantData* data = getData(variant);
    return data ? data->asIntegral<T>() : T();
  }

  static bool checkJson(VariantConstRef variant) {
    const VariantData* data = getData(variant);
    return data && data->isInteger<T>();
  }
};

template <typename T>
struct Converter<T, typename enable_if<is_enum<T>::value>::type> {
  static bool toJson(VariantRef variant, T value) {
    return variant.set(static_cast<Integer>(value));
  }

  static T fromJson(VariantConstRef variant) {
    const VariantData* data = getData(variant);
    return data ? static_cast<T>(data->asIntegral<int>()) : T();
  }

  static bool checkJson(VariantConstRef variant) {
    const VariantData* data = getData(variant);
    return data && data->isInteger<int>();
  }
};

template <>
struct Converter<bool> {
  static bool toJson(VariantRef variant, bool value) {
    VariantData* data = getData(variant);
    if (!data)
      return false;
    data->setBoolean(value);
    return true;
  }

  static bool fromJson(VariantConstRef variant) {
    const VariantData* data = getData(variant);
    return data ? data->asBoolean() : false;
  }

  static bool checkJson(VariantConstRef variant) {
    const VariantData* data = getData(variant);
    return data && data->isBoolean();
  }
};

template <typename T>
struct Converter<T, typename enable_if<is_floating_point<T>::value>::type> {
  static bool toJson(VariantRef variant, T value) {
    VariantData* data = getData(variant);
    if (!data)
      return false;
    data->setFloat(static_cast<Float>(value));
    return true;
  }

  static T fromJson(VariantConstRef variant) {
    const VariantData* data = getData(variant);
    return data ? data->asFloat<T>() : false;
  }

  static bool checkJson(VariantConstRef variant) {
    const VariantData* data = getData(variant);
    return data && data->isFloat();
  }
};

template <>
struct Converter<const char*> {
  static bool toJson(VariantRef variant, const char* value) {
    // TODO: don't pass pool
    return variantSetString(getData(variant), adaptString(value),
                            getPool(variant));
  }

  static const char* fromJson(VariantConstRef variant) {
    const VariantData* data = getData(variant);
    return data ? data->asString() : 0;
  }

  static bool checkJson(VariantConstRef variant) {
    const VariantData* data = getData(variant);
    return data && data->isString();
  }
};

template <typename T>
inline typename enable_if<IsString<T>::value, bool>::type convertToJson(
    VariantRef variant, const T& value) {
  VariantData* data = getData(variant);
  MemoryPool* pool = getPool(variant);
  return variantSetString(data, adaptString(value), pool);
}

template <typename T>
inline typename enable_if<IsWriteableString<T>::value>::type convertFromJson(
    T& value, VariantConstRef variant) {
  const VariantData* data = getData(variant);
  const char* cstr = data != 0 ? data->asString() : 0;
  if (cstr)
    value = cstr;
  else
    serializeJson(variant, value);
}

template <typename T>
inline typename enable_if<IsWriteableString<T>::value, bool>::type
canConvertFromJson(T&, VariantConstRef variant) {
  const VariantData* data = getData(variant);
  return data && data->isString();
}

template <>
struct Converter<SerializedValue<const char*> > {
  static bool toJson(VariantRef variant, SerializedValue<const char*> value) {
    VariantData* data = getData(variant);
    if (!data)
      return false;
    data->setLinkedRaw(value);
    return true;
  }
};

// SerializedValue<std::string>
// SerializedValue<String>
// SerializedValue<const __FlashStringHelper*>
template <typename T>
struct Converter<SerializedValue<T>,
                 typename enable_if<!is_same<const char*, T>::value>::type> {
  static bool toJson(VariantRef variant, SerializedValue<T> value) {
    VariantData* data = getData(variant);
    MemoryPool* pool = getPool(variant);
    return data != 0 && data->setOwnedRaw(value, pool);
  }
};

#if ARDUINOJSON_HAS_NULLPTR

template <>
struct Converter<decltype(nullptr)> {
  static bool toJson(VariantRef variant, decltype(nullptr)) {
    variantSetNull(getData(variant));
    return true;
  }
  static decltype(nullptr) fromJson(VariantConstRef) {
    return nullptr;
  }
  static bool checkJson(VariantConstRef variant) {
    const VariantData* data = getData(variant);
    return data == 0 || data->isNull();
  }
};

#endif

}  // namespace ARDUINOJSON_NAMESPACE
