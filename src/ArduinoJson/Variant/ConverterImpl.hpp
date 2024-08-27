// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Json/JsonSerializer.hpp>
#include <ArduinoJson/Memory/StringBuilder.hpp>
#include <ArduinoJson/Polyfills/utility.hpp>
#include <ArduinoJson/Variant/JsonVariantConst.hpp>

#if ARDUINOJSON_ENABLE_STD_STRING
#  include <string>
#endif

#if ARDUINOJSON_ENABLE_STRING_VIEW
#  include <string_view>
#endif

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

template <typename T, typename Enable>
struct Converter {
  static_assert(!detail::is_same<T, char>::value,
                "type 'char' is not supported, use 'signed char', 'unsigned "
                "char' or another integer type instead");

  static void toJson(const T& src, JsonVariant dst) {
    // clang-format off
    convertToJson(src, dst); // Error here? See https://arduinojson.org/v7/unsupported-set/
    // clang-format on
  }

  static T fromJson(JsonVariantConst src) {
    static_assert(!detail::is_same<T, char*>::value,
                  "type 'char*' is not supported, use 'const char*' instead");

    // clang-format off
    T result; // Error here? See https://arduinojson.org/v7/non-default-constructible/
    convertFromJson(src, result);  // Error here? See https://arduinojson.org/v7/unsupported-as/
    // clang-format on
    return result;
  }

  static bool checkJson(JsonVariantConst src) {
    static_assert(!detail::is_same<T, char*>::value,
                  "type 'char*' is not supported, use 'const char*' instead");

    T dummy = T();
    // clang-format off
    return canConvertFromJson(src, dummy);  // Error here? See https://arduinojson.org/v7/unsupported-is/
    // clang-format on
  }
};

template <typename T>
struct Converter<T, detail::enable_if_t<detail::is_integral<T>::value &&
                                        !detail::is_same<bool, T>::value &&
                                        !detail::is_same<char, T>::value>>
    : private detail::VariantAttorney {
  static bool toJson(T src, JsonVariant dst) {
    ARDUINOJSON_ASSERT_INTEGER_TYPE_IS_SUPPORTED(T);
    auto data = getData(dst);
    if (!data)
      return false;
    auto resources = getResourceManager(dst);
    data->clear(resources);
    return data->setInteger(src, resources);
  }

  static T fromJson(JsonVariantConst src) {
    ARDUINOJSON_ASSERT_INTEGER_TYPE_IS_SUPPORTED(T);
    auto data = getData(src);
    auto resources = getResourceManager(src);
    return data ? data->template asIntegral<T>(resources) : T();
  }

  static bool checkJson(JsonVariantConst src) {
    auto data = getData(src);
    auto resources = getResourceManager(src);
    return data && data->template isInteger<T>(resources);
  }
};

template <typename T>
struct Converter<T, detail::enable_if_t<detail::is_enum<T>::value>>
    : private detail::VariantAttorney {
  static bool toJson(T src, JsonVariant dst) {
    return dst.set(static_cast<JsonInteger>(src));
  }

  static T fromJson(JsonVariantConst src) {
    auto data = getData(src);
    auto resources = getResourceManager(src);
    return data ? static_cast<T>(data->template asIntegral<int>(resources))
                : T();
  }

  static bool checkJson(JsonVariantConst src) {
    auto data = getData(src);
    auto resources = getResourceManager(src);
    return data && data->template isInteger<int>(resources);
  }
};

template <>
struct Converter<bool> : private detail::VariantAttorney {
  static bool toJson(bool src, JsonVariant dst) {
    auto data = getData(dst);
    if (!data)
      return false;
    auto resources = getResourceManager(dst);
    data->clear(resources);
    data->setBoolean(src);
    return true;
  }

  static bool fromJson(JsonVariantConst src) {
    auto data = getData(src);
    auto resources = getResourceManager(src);
    return data ? data->asBoolean(resources) : false;
  }

  static bool checkJson(JsonVariantConst src) {
    auto data = getData(src);
    return data && data->isBoolean();
  }
};

template <typename T>
struct Converter<T, detail::enable_if_t<detail::is_floating_point<T>::value>>
    : private detail::VariantAttorney {
  static bool toJson(T src, JsonVariant dst) {
    auto data = getData(dst);
    if (!data)
      return false;
    auto resources = getResourceManager(dst);
    data->clear(resources);
    return data->setFloat(src, resources);
  }

  static T fromJson(JsonVariantConst src) {
    auto data = getData(src);
    auto resources = getResourceManager(src);
    return data ? data->template asFloat<T>(resources) : 0;
  }

  static bool checkJson(JsonVariantConst src) {
    auto data = getData(src);
    return data && data->isFloat();
  }
};

template <>
struct Converter<const char*> : private detail::VariantAttorney {
  static void toJson(const char* src, JsonVariant dst) {
    detail::VariantData::setString(getData(dst), detail::adaptString(src),
                                   getResourceManager(dst));
  }

  static const char* fromJson(JsonVariantConst src) {
    auto data = getData(src);
    return data ? data->asString().c_str() : 0;
  }

  static bool checkJson(JsonVariantConst src) {
    auto data = getData(src);
    return data && data->isString();
  }
};

template <>
struct Converter<JsonString> : private detail::VariantAttorney {
  static void toJson(JsonString src, JsonVariant dst) {
    detail::VariantData::setString(getData(dst), detail::adaptString(src),
                                   getResourceManager(dst));
  }

  static JsonString fromJson(JsonVariantConst src) {
    auto data = getData(src);
    return data ? data->asString() : 0;
  }

  static bool checkJson(JsonVariantConst src) {
    auto data = getData(src);
    return data && data->isString();
  }
};

template <typename T>
inline detail::enable_if_t<detail::IsString<T>::value> convertToJson(
    const T& src, JsonVariant dst) {
  using namespace detail;
  auto data = VariantAttorney::getData(dst);
  auto resources = VariantAttorney::getResourceManager(dst);
  detail::VariantData::setString(data, adaptString(src), resources);
}

// SerializedValue<std::string>
// SerializedValue<String>
// SerializedValue<const __FlashStringHelper*>
template <typename T>
struct Converter<SerializedValue<T>> : private detail::VariantAttorney {
  static void toJson(SerializedValue<T> src, JsonVariant dst) {
    detail::VariantData::setRawString(getData(dst), src,
                                      getResourceManager(dst));
  }
};

template <>
struct Converter<detail::nullptr_t> : private detail::VariantAttorney {
  static void toJson(detail::nullptr_t, JsonVariant dst) {
    detail::VariantData::clear(getData(dst), getResourceManager(dst));
  }
  static detail::nullptr_t fromJson(JsonVariantConst) {
    return nullptr;
  }
  static bool checkJson(JsonVariantConst src) {
    auto data = getData(src);
    return data == 0 || data->isNull();
  }
};

#if ARDUINOJSON_ENABLE_ARDUINO_STREAM

namespace detail {
class StringBuilderPrint : public Print {
 public:
  StringBuilderPrint(ResourceManager* resources) : copier_(resources) {
    copier_.startString();
  }

  StringNode* save() {
    ARDUINOJSON_ASSERT(!overflowed());
    return copier_.save();
  }

  size_t write(uint8_t c) {
    copier_.append(char(c));
    return copier_.isValid() ? 1 : 0;
  }

  size_t write(const uint8_t* buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
      copier_.append(char(buffer[i]));
      if (!copier_.isValid())
        return i;
    }
    return size;
  }

  bool overflowed() const {
    return !copier_.isValid();
  }

 private:
  StringBuilder copier_;
};
}  // namespace detail

inline void convertToJson(const ::Printable& src, JsonVariant dst) {
  auto resources = detail::VariantAttorney::getResourceManager(dst);
  auto data = detail::VariantAttorney::getData(dst);
  if (!resources || !data)
    return;
  data->clear(resources);
  detail::StringBuilderPrint print(resources);
  src.printTo(print);
  if (print.overflowed())
    return;
  data->setOwnedString(print.save());
}

#endif

#if ARDUINOJSON_ENABLE_ARDUINO_STRING

inline void convertFromJson(JsonVariantConst src, ::String& dst) {
  JsonString str = src.as<JsonString>();
  if (str)
    dst = str.c_str();
  else
    serializeJson(src, dst);
}

inline bool canConvertFromJson(JsonVariantConst src, const ::String&) {
  return src.is<JsonString>();
}

#endif

#if ARDUINOJSON_ENABLE_STD_STRING

inline void convertFromJson(JsonVariantConst src, std::string& dst) {
  JsonString str = src.as<JsonString>();
  if (str)
    dst.assign(str.c_str(), str.size());
  else
    serializeJson(src, dst);
}

inline bool canConvertFromJson(JsonVariantConst src, const std::string&) {
  return src.is<JsonString>();
}

#endif

#if ARDUINOJSON_ENABLE_STRING_VIEW

inline void convertFromJson(JsonVariantConst src, std::string_view& dst) {
  JsonString str = src.as<JsonString>();
  if (str)  // the standard doesn't allow passing null to the constructor
    dst = std::string_view(str.c_str(), str.size());
}

inline bool canConvertFromJson(JsonVariantConst src, const std::string_view&) {
  return src.is<JsonString>();
}

#endif

template <>
struct Converter<JsonArrayConst> : private detail::VariantAttorney {
  static void toJson(JsonArrayConst src, JsonVariant dst) {
    if (src.isNull())
      dst.set(nullptr);
    else
      dst.to<JsonArray>().set(src);
  }

  static JsonArrayConst fromJson(JsonVariantConst src) {
    auto data = getData(src);
    auto array = data ? data->asArray() : nullptr;
    return JsonArrayConst(array, getResourceManager(src));
  }

  static bool checkJson(JsonVariantConst src) {
    auto data = getData(src);
    return data && data->isArray();
  }
};

template <>
struct Converter<JsonArray> : private detail::VariantAttorney {
  static void toJson(JsonVariantConst src, JsonVariant dst) {
    if (src.isNull())
      dst.set(nullptr);
    else
      dst.to<JsonArray>().set(src);
  }

  static JsonArray fromJson(JsonVariant src) {
    auto data = getData(src);
    auto resources = getResourceManager(src);
    return JsonArray(data != 0 ? data->asArray() : 0, resources);
  }

  static bool checkJson(JsonVariant src) {
    auto data = getData(src);
    return data && data->isArray();
  }
};

template <>
struct Converter<JsonObjectConst> : private detail::VariantAttorney {
  static void toJson(JsonVariantConst src, JsonVariant dst) {
    if (src.isNull())
      dst.set(nullptr);
    else
      dst.to<JsonObject>().set(src);
  }

  static JsonObjectConst fromJson(JsonVariantConst src) {
    auto data = getData(src);
    auto object = data != 0 ? data->asObject() : nullptr;
    return JsonObjectConst(object, getResourceManager(src));
  }

  static bool checkJson(JsonVariantConst src) {
    auto data = getData(src);
    return data && data->isObject();
  }
};

template <>
struct Converter<JsonObject> : private detail::VariantAttorney {
  static void toJson(JsonVariantConst src, JsonVariant dst) {
    if (src.isNull())
      dst.set(nullptr);
    else
      dst.to<JsonObject>().set(src);
  }

  static JsonObject fromJson(JsonVariant src) {
    auto data = getData(src);
    auto resources = getResourceManager(src);
    return JsonObject(data != 0 ? data->asObject() : 0, resources);
  }

  static bool checkJson(JsonVariant src) {
    auto data = getData(src);
    return data && data->isObject();
  }
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
