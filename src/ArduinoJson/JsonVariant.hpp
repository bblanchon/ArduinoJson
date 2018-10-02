// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stddef.h>
#include <stdint.h>  // for uint8_t

#include "Data/JsonVariantData.hpp"
#include "JsonVariant.hpp"
#include "JsonVariantBase.hpp"
#include "Memory/MemoryPool.hpp"
#include "Numbers/parseFloat.hpp"
#include "Numbers/parseInteger.hpp"
#include "Polyfills/type_traits.hpp"
#include "Serialization/DynamicStringWriter.hpp"
#include "SerializedValue.hpp"

namespace ARDUINOJSON_NAMESPACE {

// Forward declarations.
class JsonArray;
class JsonObject;

// A variant that can be a any value serializable to a JSON value.
//
// It can be set to:
// - a boolean
// - a char, short, int or a long (signed or unsigned)
// - a string (const char*)
// - a reference to a JsonArray or JsonObject
class JsonVariant : public JsonVariantBase<JsonVariant> {
 public:
  // Intenal use only
  FORCE_INLINE JsonVariant(MemoryPool *memoryPool, JsonVariantData *data)
      : _memoryPool(memoryPool), _data(data) {}

  // Creates an uninitialized JsonVariant
  FORCE_INLINE JsonVariant() : _memoryPool(0), _data(0) {}

  // set(bool value)
  FORCE_INLINE bool set(bool value) {
    if (!_data) return false;
    _data->type = JSON_BOOLEAN;
    _data->content.asInteger = static_cast<JsonUInt>(value);
    return true;
  }

  // set(double value);
  // set(float value);
  template <typename T>
  FORCE_INLINE bool set(
      T value, typename enable_if<is_floating_point<T>::value>::type * = 0) {
    if (!_data) return false;
    _data->type = JSON_FLOAT;
    _data->content.asFloat = static_cast<JsonFloat>(value);
    return true;
  }

  // set(char)
  // set(signed short)
  // set(signed int)
  // set(signed long)
  // set(signed char)
  template <typename T>
  FORCE_INLINE bool set(T value,
                        typename enable_if<is_integral<T>::value &&
                                           is_signed<T>::value>::type * = 0) {
    if (!_data) return false;
    if (value >= 0) {
      _data->type = JSON_POSITIVE_INTEGER;
      _data->content.asInteger = static_cast<JsonUInt>(value);
    } else {
      _data->type = JSON_NEGATIVE_INTEGER;
      _data->content.asInteger = ~static_cast<JsonUInt>(value) + 1;
    }
    return true;
  }

  // set(unsigned short)
  // set(unsigned int)
  // set(unsigned long)
  template <typename T>
  FORCE_INLINE bool set(T value,
                        typename enable_if<is_integral<T>::value &&
                                           is_unsigned<T>::value>::type * = 0) {
    if (!_data) return false;
    _data->type = JSON_POSITIVE_INTEGER;
    _data->content.asInteger = static_cast<JsonUInt>(value);
    return true;
  }

  // set(SerializedValue<const char *>)
  FORCE_INLINE bool set(SerializedValue<const char *> value) {
    if (!_data) return false;
    _data->type = JSON_LINKED_RAW;
    _data->content.asRaw.data = value.data();
    _data->content.asRaw.size = value.size();
    return true;
  }

  // set(SerializedValue<std::string>)
  // set(SerializedValue<String>)
  // set(SerializedValue<const __FlashStringHelper*>)
  template <typename T>
  FORCE_INLINE bool set(
      SerializedValue<T> value,
      typename enable_if<!is_same<const char *, T>::value>::type * = 0) {
    if (!_data) return false;
    const char *dup = makeString(value.data(), value.size()).save(_memoryPool);
    if (dup) {
      _data->type = JSON_OWNED_RAW;
      _data->content.asRaw.data = dup;
      _data->content.asRaw.size = value.size();
      return true;
    } else {
      _data->type = JSON_NULL;
      return false;
    }
  }

  // set(const std::string&)
  // set(const String&)
  template <typename T>
  FORCE_INLINE bool set(const T &value,
                        typename enable_if<IsString<T>::value>::type * = 0) {
    if (!_data) return false;
    const char *dup = makeString(value).save(_memoryPool);
    if (dup) {
      _data->type = JSON_OWNED_STRING;
      _data->content.asString = dup;
      return true;
    } else {
      _data->type = JSON_NULL;
      return false;
    }
  }

  // set(char*)
  template <typename T>
  FORCE_INLINE bool set(T *value,
                        typename enable_if<IsString<T *>::value>::type * = 0) {
    if (!_data) return false;
    const char *dup = makeString(value).save(_memoryPool);
    if (dup) {
      _data->type = JSON_OWNED_STRING;
      _data->content.asString = dup;
      return true;
    } else {
      _data->type = JSON_NULL;
      return false;
    }
  }

  // set(const char*);
  FORCE_INLINE bool set(const char *value) {
    if (!_data) return false;
    _data->type = JSON_LINKED_STRING;
    _data->content.asString = value;
    return true;
  }

  bool set(const JsonVariant &value);

  FORCE_INLINE bool set(JsonArray array);
  FORCE_INLINE bool set(const JsonArraySubscript &);
  FORCE_INLINE bool set(JsonObject object);
  template <typename TString>
  FORCE_INLINE bool set(const JsonObjectSubscript<TString> &);

  // Get the variant as the specified type.
  //
  // char as<char>() const;
  // signed char as<signed char>() const;
  // signed short as<signed short>() const;
  // signed int as<signed int>() const;
  // signed long as<signed long>() const;
  // unsigned char as<unsigned char>() const;
  // unsigned short as<unsigned short>() const;
  // unsigned int as<unsigned int>() const;
  // unsigned long as<unsigned long>() const;
  template <typename T>
  FORCE_INLINE const typename enable_if<is_integral<T>::value, T>::type as()
      const {
    if (!_data) return 0;
    switch (_data->type) {
      case JSON_POSITIVE_INTEGER:
      case JSON_BOOLEAN:
        return T(_data->content.asInteger);
      case JSON_NEGATIVE_INTEGER:
        return T(~_data->content.asInteger + 1);
      case JSON_LINKED_STRING:
      case JSON_OWNED_STRING:
        return parseInteger<T>(_data->content.asString);
      case JSON_FLOAT:
        return T(_data->content.asFloat);
      default:
        return 0;
    }
  }
  // bool as<bool>() const
  template <typename T>
  FORCE_INLINE const typename enable_if<is_same<T, bool>::value, T>::type as()
      const {
    return as<int>() != 0;
  }
  //
  // double as<double>() const;
  // float as<float>() const;
  template <typename T>
  FORCE_INLINE const typename enable_if<is_floating_point<T>::value, T>::type
  as() const {
    if (!_data) return 0;
    switch (_data->type) {
      case JSON_POSITIVE_INTEGER:
      case JSON_BOOLEAN:
        return static_cast<T>(_data->content.asInteger);
      case JSON_NEGATIVE_INTEGER:
        return -static_cast<T>(_data->content.asInteger);
      case JSON_LINKED_STRING:
      case JSON_OWNED_STRING:
        return parseFloat<T>(_data->content.asString);
      case JSON_FLOAT:
        return static_cast<T>(_data->content.asFloat);
      default:
        return 0;
    }
  }
  //
  // const char* as<const char*>() const;
  // const char* as<char*>() const;
  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, const char *>::value ||
                                      is_same<T, char *>::value,
                                  const char *>::type
  as() const {
    if (!_data) return 0;
    if (_data &&
        (_data->type == JSON_LINKED_STRING || _data->type == JSON_OWNED_STRING))
      return _data->content.asString;
    else
      return 0;
  }
  //
  // std::string as<std::string>() const;
  // String as<String>() const;
  template <typename T>
  FORCE_INLINE typename enable_if<IsWriteableString<T>::value, T>::type as()
      const {
    const char *cstr = as<const char *>();
    if (cstr) return T(cstr);
    T s;
    serializeJson(*this, s);
    return s;
  }
  //
  // JsonArray as<JsonArray>() const;
  // const JsonArray as<const JsonArray>() const;
  template <typename T>
  FORCE_INLINE typename enable_if<
      is_same<typename remove_const<T>::type, JsonArray>::value,
      JsonArray>::type
  as() const;
  //
  // JsonObject as<JsonObject>() const;
  // const JsonObject as<const JsonObject>() const;
  template <typename T>
  FORCE_INLINE typename enable_if<
      is_same<typename remove_const<T>::type, JsonObject>::value, T>::type
  as() const;
  //
  // JsonVariant as<JsonVariant> const;
  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, JsonVariant>::value, T>::type as()
      const {
    return *this;
  }

  // Tells weither the variant has the specified type.
  // Returns true if the variant has type type T, false otherwise.
  //
  // bool is<char>() const;
  // bool is<signed char>() const;
  // bool is<signed short>() const;
  // bool is<signed int>() const;
  // bool is<signed long>() const;
  // bool is<unsigned char>() const;
  // bool is<unsigned short>() const;
  // bool is<unsigned int>() const;
  // bool is<unsigned long>() const;
  template <typename T>
  FORCE_INLINE typename enable_if<is_integral<T>::value, bool>::type is()
      const {
    return _data && (_data->type == JSON_POSITIVE_INTEGER ||
                     _data->type == JSON_NEGATIVE_INTEGER);
  }
  //
  // bool is<double>() const;
  // bool is<float>() const;
  template <typename T>
  FORCE_INLINE typename enable_if<is_floating_point<T>::value, bool>::type is()
      const {
    return _data &&
           (_data->type == JSON_FLOAT || _data->type == JSON_POSITIVE_INTEGER ||
            _data->type == JSON_NEGATIVE_INTEGER);
  }
  //
  // bool is<bool>() const
  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, bool>::value, bool>::type is()
      const {
    return _data && _data->type == JSON_BOOLEAN;
  }
  //
  // bool is<const char*>() const;
  // bool is<char*>() const;
  template <typename T>
  FORCE_INLINE typename enable_if<
      is_same<T, const char *>::value || is_same<T, char *>::value, bool>::type
  is() const {
    return _data && (_data->type == JSON_LINKED_STRING ||
                     _data->type == JSON_OWNED_STRING);
  }
  //
  // bool is<JsonArray> const;
  // bool is<const JsonArray> const;
  template <typename T>
  FORCE_INLINE typename enable_if<
      is_same<typename remove_const<T>::type, JsonArray>::value, bool>::type
  is() const {
    return _data && _data->type == JSON_ARRAY;
  }
  //
  // bool is<JsonObject> const;
  // bool is<const JsonObject> const;
  template <typename T>
  FORCE_INLINE typename enable_if<
      is_same<typename remove_const<T>::type, JsonObject>::value, bool>::type
  is() const {
    return _data && _data->type == JSON_OBJECT;
  }

  FORCE_INLINE bool isNull() const {
    return _data == 0 || _data->type == JSON_NULL;
  }

  FORCE_INLINE bool isInvalid() const {
    return _data == 0;
  }

  template <typename Visitor>
  void accept(Visitor &visitor) const;

  // Change the type of the variant
  //
  // JsonArray to<JsonArray>()
  template <typename T>
  typename enable_if<is_same<T, JsonArray>::value, JsonArray>::type to();
  //
  // JsonObject to<JsonObject>()
  template <typename T>
  typename enable_if<is_same<T, JsonObject>::value, JsonObject>::type to();
  //
  // JsonObject to<JsonVariant>()
  template <typename T>
  typename enable_if<is_same<T, JsonVariant>::value, JsonVariant>::type to();

 private:
  MemoryPool *_memoryPool;
  JsonVariantData *_data;
};

class JsonVariantLocal : public JsonVariant {
 public:
  explicit JsonVariantLocal(MemoryPool *memoryPool)
      : JsonVariant(memoryPool, &_localData) {
    _localData.type = JSON_NULL;
  }

 private:
  JsonVariantData _localData;
};
}  // namespace ARDUINOJSON_NAMESPACE
