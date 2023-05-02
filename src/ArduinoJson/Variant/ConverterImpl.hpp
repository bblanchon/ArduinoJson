// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Json/JsonSerializer.hpp>
#include <ArduinoJson/StringStorage/StringCopier.hpp>
#include <ArduinoJson/Variant/JsonVariantConst.hpp>
#include <ArduinoJson/Variant/VariantFunctions.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

template <typename T, typename Enable>
struct Converter {
  static void toJson(const T& src, JsonVariant dst) {
    // clang-format off
    convertToJson(src, dst); // Error here? See https://arduinojson.org/v6/unsupported-set/
    // clang-format on
  }

  static T fromJson(JsonVariantConst src) {
    // clang-format off
    T result; // Error here? See https://arduinojson.org/v6/non-default-constructible/
    convertFromJson(src, result);  // Error here? See https://arduinojson.org/v6/unsupported-as/
    // clang-format on
    return result;
  }

  static bool checkJson(JsonVariantConst src) {
    T dummy = T();
    // clang-format off
    return canConvertFromJson(src, dummy);  // Error here? See https://arduinojson.org/v6/unsupported-is/
    // clang-format on
  }
};

template <typename T>
struct Converter<
    T, typename detail::enable_if<detail::is_integral<T>::value &&
                                  !detail::is_same<bool, T>::value &&
                                  !detail::is_same<char, T>::value>::type>
    : private detail::VariantAttorney {
  static void toJson(T src, JsonVariant dst) {
    ARDUINOJSON_ASSERT_INTEGER_TYPE_IS_SUPPORTED(T);
    variantSetInteger(getData(dst), src, getPool(dst));
  }

  static T fromJson(JsonVariantConst src) {
    ARDUINOJSON_ASSERT_INTEGER_TYPE_IS_SUPPORTED(T);
    auto data = getData(src);
    return data ? data->template asIntegral<T>() : T();
  }

  static bool checkJson(JsonVariantConst src) {
    auto data = getData(src);
    return data && data->template isInteger<T>();
  }
};

template <typename T>
struct Converter<T, typename detail::enable_if<detail::is_enum<T>::value>::type>
    : private detail::VariantAttorney {
  static void toJson(T src, JsonVariant dst) {
    dst.set(static_cast<JsonInteger>(src));
  }

  static T fromJson(JsonVariantConst src) {
    auto data = getData(src);
    return data ? static_cast<T>(data->template asIntegral<int>()) : T();
  }

  static bool checkJson(JsonVariantConst src) {
    auto data = getData(src);
    return data && data->template isInteger<int>();
  }
};

template <>
struct Converter<bool> : private detail::VariantAttorney {
  static void toJson(bool src, JsonVariant dst) {
    variantSetBoolean(getData(dst), src, getPool(dst));
  }

  static bool fromJson(JsonVariantConst src) {
    auto data = getData(src);
    return data ? data->asBoolean() : false;
  }

  static bool checkJson(JsonVariantConst src) {
    auto data = getData(src);
    return data && data->isBoolean();
  }
};

template <typename T>
struct Converter<
    T, typename detail::enable_if<detail::is_floating_point<T>::value>::type>
    : private detail::VariantAttorney {
  static void toJson(T src, JsonVariant dst) {
    variantSetFloat(getData(dst), static_cast<JsonFloat>(src), getPool(dst));
  }

  static T fromJson(JsonVariantConst src) {
    auto data = getData(src);
    return data ? data->template asFloat<T>() : 0;
  }

  static bool checkJson(JsonVariantConst src) {
    auto data = getData(src);
    return data && data->isFloat();
  }
};

template <>
struct Converter<const char*> : private detail::VariantAttorney {
  static void toJson(const char* src, JsonVariant dst) {
    variantSetString(getData(dst), detail::adaptString(src), getPool(dst));
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
    variantSetString(getData(dst), detail::adaptString(src), getPool(dst));
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
inline typename detail::enable_if<detail::IsString<T>::value>::type
convertToJson(const T& src, JsonVariant dst) {
  using namespace detail;
  auto data = VariantAttorney::getData(dst);
  auto pool = VariantAttorney::getPool(dst);
  variantSetString(data, adaptString(src), pool);
}

// SerializedValue<std::string>
// SerializedValue<String>
// SerializedValue<const __FlashStringHelper*>
template <typename T>
struct Converter<SerializedValue<T>> : private detail::VariantAttorney {
  static void toJson(SerializedValue<T> src, JsonVariant dst) {
    variantSetOwnedRaw(getData(dst), src, getPool(dst));
  }
};

template <>
struct Converter<decltype(nullptr)> : private detail::VariantAttorney {
  static void toJson(decltype(nullptr), JsonVariant dst) {
    variantSetNull(getData(dst), getPool(dst));
  }
  static decltype(nullptr) fromJson(JsonVariantConst) {
    return nullptr;
  }
  static bool checkJson(JsonVariantConst src) {
    auto data = getData(src);
    return data == 0 || data->isNull();
  }
};

#if ARDUINOJSON_ENABLE_ARDUINO_STREAM

namespace detail {
class MemoryPoolPrint : public Print {
 public:
  MemoryPoolPrint(MemoryPool* pool) : copier_(pool) {
    copier_.startString();
  }

  JsonString str() {
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
  StringCopier copier_;
};
}  // namespace detail

inline void convertToJson(const ::Printable& src, JsonVariant dst) {
  auto pool = detail::VariantAttorney::getPool(dst);
  auto data = detail::VariantAttorney::getData(dst);
  if (!pool || !data)
    return;
  detail::MemoryPoolPrint print(pool);
  src.printTo(print);
  if (print.overflowed()) {
    data->setNull();
    return;
  }
  data->setString(print.str());
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

namespace detail {
template <typename T>
struct ConverterNeedsWriteableRef {
 protected:  // <- to avoid GCC's "all member functions in class are private"
  static int probe(T (*f)(ArduinoJson::JsonVariant));
  static char probe(T (*f)(ArduinoJson::JsonVariantConst));

 public:
  static const bool value =
      sizeof(probe(Converter<T>::fromJson)) == sizeof(int);
};
}  // namespace detail

ARDUINOJSON_END_PUBLIC_NAMESPACE
