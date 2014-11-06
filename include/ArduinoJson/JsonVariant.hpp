// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include <stddef.h>
#include <stdint.h>  // for uint8_t

#include "Internals/JsonPrintable.hpp"
#include "Internals/JsonVariantContent.hpp"
#include "Internals/JsonVariantType.hpp"

namespace ArduinoJson {

class JsonArray;
class JsonObject;

class JsonVariant : public Internals::JsonPrintable<JsonVariant> {
 public:
  JsonVariant() : _type(Internals::JSON_UNDEFINED) {}

  template <typename T>
  explicit JsonVariant(T value) {
    set(value);
  }

  void set(bool value);
  void set(double value, uint8_t decimals = 2);
  void set(signed char value) { set(static_cast<long>(value)); }
  void set(signed int value) { set(static_cast<long>(value)); }
  void set(signed long value);
  void set(signed short value) { set(static_cast<long>(value)); }
  void set(unsigned char value) { set(static_cast<long>(value)); }
  void set(unsigned int value) { set(static_cast<long>(value)); }
  void set(unsigned long value) { set(static_cast<long>(value)); }
  void set(unsigned short value) { set(static_cast<long>(value)); }
  void set(const char *value);
  void set(JsonArray &array);
  void set(JsonObject &object);

  template <typename T>
  JsonVariant &operator=(T value) {
    set(value);
    return *this;
  }

  JsonVariant &operator=(JsonArray &array) {
    set(array);
    return *this;
  }

  JsonVariant &operator=(JsonObject &object) {
    set(object);
    return *this;
  }

  operator bool() const;
  operator double() const;
  operator float() const { return static_cast<float>(as<double>()); }
  operator signed char() const { return static_cast<signed char>(as<long>()); }
  operator signed int() const { return static_cast<signed int>(as<long>()); }
  operator signed long() const;
  operator signed short() const {
    return static_cast<signed short>(as<long>());
  }
  operator unsigned char() const {
    return static_cast<unsigned char>(as<long>());
  }
  operator unsigned int() const {
    return static_cast<unsigned int>(as<long>());
  }
  operator unsigned long() const {
    return static_cast<unsigned long>(as<long>());
  }
  operator unsigned short() const {
    return static_cast<unsigned short>(as<long>());
  }
  operator const char *() const;
  operator JsonArray &() const;
  operator JsonObject &() const;

  const char *asString() const { return this->as<const char *>(); }
  JsonArray &asArray() const { return this->as<JsonArray &>(); }
  JsonObject &asObject() const { return this->as<JsonObject &>(); }

  template <typename T>
  T as() const {
    return static_cast<T>(*this);
  }

  template <typename T>
  bool is() const {
    return false;
  }

  static JsonVariant &invalid() { return _invalid; }

  bool success() {
    return _type != Internals::JSON_INVALID &&
           _type != Internals::JSON_UNDEFINED;
  }

  template <typename T>
  void writeTo(T &writer) const;

  size_t size() const;
  JsonVariant &operator[](int index);
  JsonVariant &operator[](const char *key);

 private:
  explicit JsonVariant(Internals::JsonVariantType type) : _type(type) {}

  Internals::JsonVariantType _type;
  Internals::JsonVariantContent _content;
  static JsonVariant _invalid;
};

template <>
inline bool JsonVariant::is<long>() const {
  return _type == Internals::JSON_LONG;
}

template <>
inline bool JsonVariant::is<double>() const {
  return _type >= Internals::JSON_DOUBLE_0_DECIMALS;
}

template <typename T>
inline bool operator==(const JsonVariant &left, T right) {
  return left.as<T>() == right;
}

template <typename T>
inline bool operator==(T left, const JsonVariant &right) {
  return left == right.as<T>();
}

template <typename T>
inline bool operator!=(const JsonVariant &left, T right) {
  return left.as<T>() != right;
}

template <typename T>
inline bool operator!=(T left, const JsonVariant &right) {
  return left != right.as<T>();
}

template <typename T>
inline bool operator<=(const JsonVariant &left, T right) {
  return left.as<T>() <= right;
}

template <typename T>
inline bool operator<=(T left, const JsonVariant &right) {
  return left <= right.as<T>();
}

template <typename T>
inline bool operator>=(const JsonVariant &left, T right) {
  return left.as<T>() >= right;
}

template <typename T>
inline bool operator>=(T left, const JsonVariant &right) {
  return left >= right.as<T>();
}

template <typename T>
inline bool operator<(const JsonVariant &left, T right) {
  return left.as<T>() < right;
}

template <typename T>
inline bool operator<(T left, const JsonVariant &right) {
  return left < right.as<T>();
}

template <typename T>
inline bool operator>(const JsonVariant &left, T right) {
  return left.as<T>() > right;
}

template <typename T>
inline bool operator>(T left, const JsonVariant &right) {
  return left > right.as<T>();
}
}
