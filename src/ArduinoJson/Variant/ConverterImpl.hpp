// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Json/JsonSerializer.hpp>
#include <ArduinoJson/Variant/VariantFunctions.hpp>
#include <ArduinoJson/Variant/VariantRef.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename T, typename Enable>
struct Converter {
  static void toJson(const T& src, VariantRef dst) {
    // clang-format off
    convertToJson(src, dst); // Error here? See https://arduinojson.org/v6/unsupported-set/
    // clang-format on
  }

  static T fromJson(VariantConstRef src) {
    // clang-format off
    T result; // Error here? See https://arduinojson.org/v6/non-default-constructible/
    convertFromJson(src, result);  // Error here? See https://arduinojson.org/v6/unsupported-as/
    // clang-format on
    return result;
  }

  static bool checkJson(VariantConstRef src) {
    T dummy = T();
    // clang-format off
    return canConvertFromJson(src, dummy);  // Error here? See https://arduinojson.org/v6/unsupported-is/
    // clang-format on
  }
};

template <typename T>
struct Converter<
    T, typename enable_if<is_integral<T>::value && !is_same<bool, T>::value &&
                          !is_same<char, T>::value>::type> {
  static void toJson(T src, VariantRef dst) {
    VariantData* data = getData(dst);
    ARDUINOJSON_ASSERT_INTEGER_TYPE_IS_SUPPORTED(T);
    if (data)
      data->setInteger(src);
  }

  static T fromJson(VariantConstRef src) {
    ARDUINOJSON_ASSERT_INTEGER_TYPE_IS_SUPPORTED(T);
    const VariantData* data = getData(src);
    return data ? data->asIntegral<T>() : T();
  }

  static bool checkJson(VariantConstRef src) {
    const VariantData* data = getData(src);
    return data && data->isInteger<T>();
  }
};

template <typename T>
struct Converter<T, typename enable_if<is_enum<T>::value>::type> {
  static void toJson(T src, VariantRef dst) {
    dst.set(static_cast<Integer>(src));
  }

  static T fromJson(VariantConstRef src) {
    const VariantData* data = getData(src);
    return data ? static_cast<T>(data->asIntegral<int>()) : T();
  }

  static bool checkJson(VariantConstRef src) {
    const VariantData* data = getData(src);
    return data && data->isInteger<int>();
  }
};

template <>
struct Converter<bool> {
  static void toJson(bool src, VariantRef dst) {
    VariantData* data = getData(dst);
    if (data)
      data->setBoolean(src);
  }

  static bool fromJson(VariantConstRef src) {
    const VariantData* data = getData(src);
    return data ? data->asBoolean() : false;
  }

  static bool checkJson(VariantConstRef src) {
    const VariantData* data = getData(src);
    return data && data->isBoolean();
  }
};

template <typename T>
struct Converter<T, typename enable_if<is_floating_point<T>::value>::type> {
  static void toJson(T src, VariantRef dst) {
    VariantData* data = getData(dst);
    if (data)
      data->setFloat(static_cast<Float>(src));
  }

  static T fromJson(VariantConstRef src) {
    const VariantData* data = getData(src);
    return data ? data->asFloat<T>() : false;
  }

  static bool checkJson(VariantConstRef src) {
    const VariantData* data = getData(src);
    return data && data->isFloat();
  }
};

template <>
struct Converter<const char*> {
  static void toJson(const char* src, VariantRef dst) {
    variantSetString(getData(dst), adaptString(src), getPool(dst),
                     getStringStoragePolicy(src));
  }

  static const char* fromJson(VariantConstRef src) {
    const VariantData* data = getData(src);
    return data ? data->asString().c_str() : 0;
  }

  static bool checkJson(VariantConstRef src) {
    const VariantData* data = getData(src);
    return data && data->isString();
  }
};

template <>
struct Converter<String> {
  static void toJson(String src, VariantRef dst) {
    variantSetString(getData(dst), adaptString(src), getPool(dst),
                     getStringStoragePolicy(src));
  }

  static String fromJson(VariantConstRef src) {
    const VariantData* data = getData(src);
    return data ? data->asString() : 0;
  }

  static bool checkJson(VariantConstRef src) {
    const VariantData* data = getData(src);
    return data && data->isString();
  }
};

template <typename T>
inline typename enable_if<IsString<T>::value, bool>::type convertToJson(
    const T& src, VariantRef dst) {
  VariantData* data = getData(dst);
  MemoryPool* pool = getPool(dst);
  return variantSetString(data, adaptString(src), pool,
                          getStringStoragePolicy(src));
}

template <>
struct Converter<SerializedValue<const char*> > {
  static void toJson(SerializedValue<const char*> src, VariantRef dst) {
    VariantData* data = getData(dst);
    if (data)
      data->setLinkedRaw(src);
  }
};

// SerializedValue<std::string>
// SerializedValue<String>
// SerializedValue<const __FlashStringHelper*>
template <typename T>
struct Converter<SerializedValue<T>,
                 typename enable_if<!is_same<const char*, T>::value>::type> {
  static void toJson(SerializedValue<T> src, VariantRef dst) {
    VariantData* data = getData(dst);
    MemoryPool* pool = getPool(dst);
    if (data)
      data->storeOwnedRaw(src, pool);
  }
};

#if ARDUINOJSON_HAS_NULLPTR

template <>
struct Converter<decltype(nullptr)> {
  static void toJson(decltype(nullptr), VariantRef dst) {
    variantSetNull(getData(dst));
  }
  static decltype(nullptr) fromJson(VariantConstRef) {
    return nullptr;
  }
  static bool checkJson(VariantConstRef src) {
    const VariantData* data = getData(src);
    return data == 0 || data->isNull();
  }
};

#endif

#if ARDUINOJSON_ENABLE_ARDUINO_STREAM

class MemoryPoolPrint : public Print {
 public:
  MemoryPoolPrint(MemoryPool* pool) : _pool(pool), _size(0) {
    pool->getFreeZone(&_string, &_capacity);
  }

  CopiedString str() {
    ARDUINOJSON_ASSERT(_size < _capacity);
    return CopiedString(_pool->saveStringFromFreeZone(_size), _size);
  }

  size_t write(uint8_t c) {
    if (_size >= _capacity)
      return 0;

    _string[_size++] = char(c);
    return 1;
  }

  size_t write(const uint8_t* buffer, size_t size) {
    if (_size + size >= _capacity) {
      _size = _capacity;  // mark as overflowed
      return 0;
    }
    memcpy(&_string[_size], buffer, size);
    _size += size;
    return size;
  }

  bool overflowed() const {
    return _size >= _capacity;
  }

 private:
  MemoryPool* _pool;
  size_t _size;
  char* _string;
  size_t _capacity;
};

inline void convertToJson(const ::Printable& src, VariantRef dst) {
  MemoryPool* pool = getPool(dst);
  VariantData* data = getData(dst);
  if (!pool || !data)
    return;
  MemoryPoolPrint print(pool);
  src.printTo(print);
  if (print.overflowed()) {
    pool->markAsOverflowed();
    data->setNull();
    return;
  }
  data->setString(print.str());
}

#endif

#if ARDUINOJSON_ENABLE_ARDUINO_STRING

inline void convertFromJson(VariantConstRef src, ::String& dst) {
  String str = src.as<String>();
  if (str)
    dst = str.c_str();
  else
    serializeJson(src, dst);
}

inline bool canConvertFromJson(VariantConstRef src, const ::String&) {
  return src.is<String>();
}

#endif

#if ARDUINOJSON_ENABLE_STD_STRING

inline void convertFromJson(VariantConstRef src, std::string& dst) {
  String str = src.as<String>();
  if (str)
    dst.assign(str.c_str(), str.size());
  else
    serializeJson(src, dst);
}

inline bool canConvertFromJson(VariantConstRef src, const std::string&) {
  return src.is<String>();
}

#endif

#if ARDUINOJSON_ENABLE_STRING_VIEW

inline void convertFromJson(VariantConstRef src, std::string_view& dst) {
  String str = src.as<String>();
  if (str)  // the standard doesn't allow passing null to the constructor
    dst = std::string_view(str.c_str(), str.size());
}

inline bool canConvertFromJson(VariantConstRef src, const std::string_view&) {
  return src.is<String>();
}

#endif

}  // namespace ARDUINOJSON_NAMESPACE
