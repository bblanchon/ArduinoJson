// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stddef.h>
#include <stdint.h>  // for uint8_t

#include "Data/JsonVariantContent.hpp"
#include "Data/JsonVariantDefault.hpp"
#include "Data/JsonVariantType.hpp"
#include "JsonVariantBase.hpp"
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
  // Creates an uninitialized JsonVariant
  JsonVariant() : _type(Internals::JSON_UNDEFINED) {}

  // Create a JsonVariant containing a boolean value.
  // It will be serialized as "true" or "false" in JSON.
  JsonVariant(bool value) {
    using namespace Internals;
    _type = JSON_BOOLEAN;
    _content.asInteger = static_cast<JsonUInt>(value);
  }

  // Create a JsonVariant containing a floating point value.
  // JsonVariant(double value);
  // JsonVariant(float value);
  template <typename T>
  JsonVariant(T value,
              typename Internals::enable_if<
                  Internals::is_floating_point<T>::value>::type * = 0) {
    using namespace Internals;
    _type = JSON_FLOAT;
    _content.asFloat = static_cast<JsonFloat>(value);
  }

  // Create a JsonVariant containing an integer value.
  // JsonVariant(char)
  // JsonVariant(signed short)
  // JsonVariant(signed int)
  // JsonVariant(signed long)
  // JsonVariant(signed char)
  template <typename T>
  JsonVariant(
      T value,
      typename Internals::enable_if<Internals::is_integral<T>::value &&
                                    Internals::is_signed<T>::value>::type * =
          0) {
    using namespace Internals;
    if (value >= 0) {
      _type = JSON_POSITIVE_INTEGER;
      _content.asInteger = static_cast<JsonUInt>(value);
    } else {
      _type = JSON_NEGATIVE_INTEGER;
      _content.asInteger = ~static_cast<JsonUInt>(value) + 1;
    }
  }
  // JsonVariant(unsigned short)
  // JsonVariant(unsigned int)
  // JsonVariant(unsigned long)
  template <typename T>
  JsonVariant(
      T value,
      typename Internals::enable_if<Internals::is_integral<T>::value &&
                                    Internals::is_unsigned<T>::value>::type * =
          0) {
    using namespace Internals;
    _type = JSON_POSITIVE_INTEGER;
    _content.asInteger = static_cast<JsonUInt>(value);
  }

  // Create a JsonVariant containing a string.
  // JsonVariant(const char*);
  // JsonVariant(const signed char*);
  // JsonVariant(const unsigned char*);
  template <typename TChar>
  JsonVariant(const TChar *value,
              typename Internals::enable_if<sizeof(TChar) == 1>::type * = 0) {
    _type = Internals::JSON_STRING;
    _content.asString = reinterpret_cast<const char *>(value);
  }

  // Create a JsonVariant containing an unparsed string
  JsonVariant(Internals::SerializedValue<const char *> value) {
    _type = Internals::JSON_UNPARSED;
    _content.asRaw.data = value.data();
    _content.asRaw.size = value.size();
  }

  JsonVariant(JsonArray array);
  JsonVariant(JsonObject object);

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
  const typename Internals::enable_if<Internals::is_integral<T>::value, T>::type
  as() const {
    return variantAsInteger<T>();
  }
  // bool as<bool>() const
  template <typename T>
  const typename Internals::enable_if<Internals::is_same<T, bool>::value,
                                      T>::type
  as() const {
    return variantAsInteger<int>() != 0;
  }
  //
  // double as<double>() const;
  // float as<float>() const;
  template <typename T>
  const typename Internals::enable_if<Internals::is_floating_point<T>::value,
                                      T>::type
  as() const {
    return variantAsFloat<T>();
  }
  //
  // const char* as<const char*>() const;
  // const char* as<char*>() const;
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, const char *>::value ||
                                    Internals::is_same<T, char *>::value,
                                const char *>::type
  as() const {
    return variantAsString();
  }
  //
  // std::string as<std::string>() const;
  // String as<String>() const;
  template <typename T>
  typename Internals::enable_if<Internals::IsWriteableString<T>::value, T>::type
  as() const {
    const char *cstr = variantAsString();
    if (cstr) return T(cstr);
    T s;
    serializeJson(*this, s);
    return s;
  }
  //
  // JsonArray as<JsonArray>() const;
  // const JsonArray as<const JsonArray>() const;
  template <typename T>
  typename Internals::enable_if<
      Internals::is_same<typename Internals::remove_const<T>::type,
                         JsonArray>::value,
      JsonArray>::type
  as() const;
  //
  // JsonObject as<JsonObject>() const;
  // const JsonObject as<const JsonObject>() const;
  template <typename T>
  typename Internals::enable_if<
      Internals::is_same<typename Internals::remove_const<T>::type,
                         JsonObject>::value,
      T>::type
  as() const;
  //
  // JsonVariant as<JsonVariant> const;
  template <typename T>
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
  typename Internals::enable_if<Internals::is_integral<T>::value, bool>::type
  is() const {
    return variantIsInteger();
  }
  //
  // bool is<double>() const;
  // bool is<float>() const;
  template <typename T>
  typename Internals::enable_if<Internals::is_floating_point<T>::value,
                                bool>::type
  is() const {
    return variantIsFloat();
  }
  //
  // bool is<bool>() const
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, bool>::value, bool>::type
  is() const {
    return variantIsBoolean();
  }
  //
  // bool is<const char*>() const;
  // bool is<char*>() const;
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, const char *>::value ||
                                    Internals::is_same<T, char *>::value,
                                bool>::type
  is() const {
    return variantIsString();
  }
  //
  // bool is<JsonArray> const;
  // bool is<const JsonArray> const;
  template <typename T>
  typename Internals::enable_if<
      Internals::is_same<typename Internals::remove_const<T>::type,
                         JsonArray>::value,
      bool>::type
  is() const {
    return variantIsArray();
  }
  //
  // bool is<JsonObject> const;
  // bool is<const JsonObject> const;
  template <typename T>
  typename Internals::enable_if<
      Internals::is_same<typename Internals::remove_const<T>::type,
                         JsonObject>::value,
      bool>::type
  is() const {
    return variantIsObject();
  }

  // Returns true if the variant has a value
  bool isNull() const {
    return _type == Internals::JSON_UNDEFINED;
  }

  template <typename Visitor>
  void visit(Visitor &visitor) const {
    using namespace Internals;
    switch (_type) {
      case JSON_FLOAT:
        return visitor.acceptFloat(_content.asFloat);

      case JSON_ARRAY:
        return visitor.acceptArray(_content.asArray);

      case JSON_OBJECT:
        return visitor.acceptObject(_content.asObject);

      case JSON_STRING:
        return visitor.acceptString(_content.asString);

      case JSON_UNPARSED:
        return visitor.acceptRawJson(_content.asRaw.data, _content.asRaw.size);

      case JSON_NEGATIVE_INTEGER:
        return visitor.acceptNegativeInteger(_content.asInteger);

      case JSON_POSITIVE_INTEGER:
        return visitor.acceptPositiveInteger(_content.asInteger);

      case JSON_BOOLEAN:
        return visitor.acceptBoolean(_content.asInteger != 0);

      default:  // JSON_UNDEFINED
        return visitor.acceptNull();
    }
  }

 private:
  JsonArray variantAsArray() const;
  JsonObject variantAsObject() const;
  const char *variantAsString() const;
  template <typename T>
  T variantAsFloat() const;
  template <typename T>
  T variantAsInteger() const;
  bool variantIsBoolean() const;
  bool variantIsFloat() const;
  bool variantIsInteger() const;
  bool variantIsArray() const {
    return _type == Internals::JSON_ARRAY;
  }
  bool variantIsObject() const {
    return _type == Internals::JSON_OBJECT;
  }
  bool variantIsString() const {
    return _type == Internals::JSON_STRING;
  }

  // The current type of the variant
  Internals::JsonVariantType _type;

  // The various alternatives for the value of the variant.
  Internals::JsonVariantContent _content;
};
}  // namespace ArduinoJson
