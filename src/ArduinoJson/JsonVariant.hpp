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
#include "Polyfills/type_traits.hpp"
#include "Serialization/DynamicStringWriter.hpp"
#include "SerializedValue.hpp"

namespace ArduinoJson {

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
class JsonVariant : public Internals::JsonVariantBase<JsonVariant> {
 public:
  // Intenal use only
  FORCE_INLINE JsonVariant(Internals::MemoryPool *memoryPool,
                           Internals::JsonVariantData *data)
      : _memoryPool(memoryPool), _data(data) {}

  // Creates an uninitialized JsonVariant
  FORCE_INLINE JsonVariant() : _memoryPool(0), _data(0) {}

  // set(bool value)
  FORCE_INLINE bool set(bool value) {
    if (!_data) return false;
    _data->setBoolean(value);
    return true;
  }

  // set(double value);
  // set(float value);
  template <typename T>
  FORCE_INLINE bool set(
      T value, typename Internals::enable_if<
                   Internals::is_floating_point<T>::value>::type * = 0) {
    if (!_data) return false;
    _data->setFloat(static_cast<Internals::JsonFloat>(value));
    return true;
  }

  // set(char)
  // set(signed short)
  // set(signed int)
  // set(signed long)
  // set(signed char)
  template <typename T>
  FORCE_INLINE bool set(
      T value,
      typename Internals::enable_if<Internals::is_integral<T>::value &&
                                    Internals::is_signed<T>::value>::type * =
          0) {
    if (!_data) return false;
    if (value >= 0)
      _data->setPostiveInteger(static_cast<Internals::JsonUInt>(value));
    else
      _data->setNegativeInteger(~static_cast<Internals::JsonUInt>(value) + 1);
    return true;
  }

  // set(unsigned short)
  // set(unsigned int)
  // set(unsigned long)
  template <typename T>
  FORCE_INLINE bool set(
      T value,
      typename Internals::enable_if<Internals::is_integral<T>::value &&
                                    Internals::is_unsigned<T>::value>::type * =
          0) {
    if (!_data) return false;
    _data->setPostiveInteger(static_cast<Internals::JsonUInt>(value));
    return true;
  }

  // set(SerializedValue<const char *>)
  FORCE_INLINE bool set(Internals::SerializedValue<const char *> value) {
    if (!_data) return false;
    _data->setLinkedRaw(value.data(), value.size());
    return true;
  }

  // set(SerializedValue<std::string>)
  // set(SerializedValue<String>)
  // set(SerializedValue<const __FlashStringHelper*>)
  template <typename T>
  FORCE_INLINE bool set(
      Internals::SerializedValue<T> value,
      typename Internals::enable_if<
          !Internals::is_same<const char *, T>::value>::type * = 0) {
    if (!_data) return false;
    const char *dup =
        Internals::makeString(value.data(), value.size()).save(_memoryPool);
    if (dup)
      _data->setOwnedRaw(dup, value.size());
    else
      _data->setNull();
    return true;
  }

  // set(const std::string&)
  // set(const String&)
  template <typename T>
  FORCE_INLINE bool set(
      const T &value,
      typename Internals::enable_if<Internals::IsString<T>::value>::type * =
          0) {
    if (!_data) return false;
    const char *dup = Internals::makeString(value).save(_memoryPool);
    if (dup) {
      _data->setOwnedString(dup);
      return true;
    } else {
      _data->setNull();
      return false;
    }
  }

  // set(char*)
  template <typename T>
  FORCE_INLINE bool set(
      T *value,
      typename Internals::enable_if<Internals::IsString<T *>::value>::type * =
          0) {
    if (!_data) return false;
    const char *dup = Internals::makeString(value).save(_memoryPool);
    if (dup) {
      _data->setOwnedString(dup);
      return true;
    } else {
      _data->setNull();
      return false;
    }
  }

  // set(const char*);
  FORCE_INLINE bool set(const char *value) {
    if (!_data) return false;
    _data->setLinkedString(value);
    return true;
  }

  bool set(const JsonVariant &value);

  FORCE_INLINE bool set(JsonArray array);
  FORCE_INLINE bool set(const Internals::JsonArraySubscript &);
  FORCE_INLINE bool set(JsonObject object);
  template <typename TString>
  FORCE_INLINE bool set(const Internals::JsonObjectSubscript<TString> &);

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
  FORCE_INLINE const typename Internals::enable_if<
      Internals::is_integral<T>::value, T>::type
  as() const {
    return _data ? _data->asInteger<T>() : T();
  }
  // bool as<bool>() const
  template <typename T>
  FORCE_INLINE const typename Internals::enable_if<
      Internals::is_same<T, bool>::value, T>::type
  as() const {
    return _data && _data->asInteger<int>() != 0;
  }
  //
  // double as<double>() const;
  // float as<float>() const;
  template <typename T>
  FORCE_INLINE const typename Internals::enable_if<
      Internals::is_floating_point<T>::value, T>::type
  as() const {
    return _data ? _data->asFloat<T>() : 0;
  }
  //
  // const char* as<const char*>() const;
  // const char* as<char*>() const;
  template <typename T>
  FORCE_INLINE typename Internals::enable_if<
      Internals::is_same<T, const char *>::value ||
          Internals::is_same<T, char *>::value,
      const char *>::type
  as() const {
    return _data ? _data->asString() : 0;
  }
  //
  // std::string as<std::string>() const;
  // String as<String>() const;
  template <typename T>
  FORCE_INLINE
      typename Internals::enable_if<Internals::IsWriteableString<T>::value,
                                    T>::type
      as() const {
    const char *cstr = _data ? _data->asString() : 0;
    if (cstr) return T(cstr);
    T s;
    serializeJson(*this, s);
    return s;
  }
  //
  // JsonArray as<JsonArray>() const;
  // const JsonArray as<const JsonArray>() const;
  template <typename T>
  FORCE_INLINE typename Internals::enable_if<
      Internals::is_same<typename Internals::remove_const<T>::type,
                         JsonArray>::value,
      JsonArray>::type
  as() const;
  //
  // JsonObject as<JsonObject>() const;
  // const JsonObject as<const JsonObject>() const;
  template <typename T>
  FORCE_INLINE typename Internals::enable_if<
      Internals::is_same<typename Internals::remove_const<T>::type,
                         JsonObject>::value,
      T>::type
  as() const;
  //
  // JsonVariant as<JsonVariant> const;
  template <typename T>
  FORCE_INLINE
      typename Internals::enable_if<Internals::is_same<T, JsonVariant>::value,
                                    T>::type
      as() const {
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
  FORCE_INLINE typename Internals::enable_if<Internals::is_integral<T>::value,
                                             bool>::type
  is() const {
    return _data && _data->isInteger();
  }
  //
  // bool is<double>() const;
  // bool is<float>() const;
  template <typename T>
  FORCE_INLINE
      typename Internals::enable_if<Internals::is_floating_point<T>::value,
                                    bool>::type
      is() const {
    return _data && _data->isFloat();
  }
  //
  // bool is<bool>() const
  template <typename T>
  FORCE_INLINE typename Internals::enable_if<Internals::is_same<T, bool>::value,
                                             bool>::type
  is() const {
    return _data && _data->isBoolean();
  }
  //
  // bool is<const char*>() const;
  // bool is<char*>() const;
  template <typename T>
  FORCE_INLINE typename Internals::enable_if<
      Internals::is_same<T, const char *>::value ||
          Internals::is_same<T, char *>::value,
      bool>::type
  is() const {
    return _data && _data->isString();
  }
  //
  // bool is<JsonArray> const;
  // bool is<const JsonArray> const;
  template <typename T>
  FORCE_INLINE typename Internals::enable_if<
      Internals::is_same<typename Internals::remove_const<T>::type,
                         JsonArray>::value,
      bool>::type
  is() const {
    return _data && _data->isArray();
  }
  //
  // bool is<JsonObject> const;
  // bool is<const JsonObject> const;
  template <typename T>
  FORCE_INLINE typename Internals::enable_if<
      Internals::is_same<typename Internals::remove_const<T>::type,
                         JsonObject>::value,
      bool>::type
  is() const {
    return _data && _data->isObject();
  }

  FORCE_INLINE bool isNull() const {
    return _data == 0 || _data->isNull();
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
  typename Internals::enable_if<Internals::is_same<T, JsonArray>::value,
                                JsonArray>::type
  to();
  //
  // JsonObject to<JsonObject>()
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, JsonObject>::value,
                                JsonObject>::type
  to();
  //
  // JsonObject to<JsonVariant>()
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, JsonVariant>::value,
                                JsonVariant>::type
  to();

 private:
  Internals::MemoryPool *_memoryPool;
  Internals::JsonVariantData *_data;
};  // namespace ArduinoJson
}  // namespace ArduinoJson
