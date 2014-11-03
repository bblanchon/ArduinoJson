// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include <stddef.h>

#include "Internals/JsonValueContent.hpp"
#include "Internals/JsonValueType.hpp"

namespace ArduinoJson {

class JsonArray;
class JsonObject;

namespace Internals {
class JsonWriter;
}

class JsonValue {
 public:
  JsonValue() : _type(Internals::JSON_UNDEFINED) {}

  void set(bool value);
  void set(double value, int decimals = 2);
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
  JsonValue &operator=(T value) {
    set(value);
    return *this;
  }

  JsonValue &operator=(JsonArray &array) {
    set(array);
    return *this;
  }

  JsonValue &operator=(JsonObject &object) {
    set(object);
    return *this;
  }

  operator bool() const;
  operator double() const;
  operator float() const { return as<double>(); }
  operator signed char() const { return as<long>(); }
  operator signed int() const { return as<long>(); }
  operator signed long() const;
  operator signed short() const { return as<long>(); }
  operator unsigned char() const { return as<long>(); }
  operator unsigned int() const { return as<long>(); }
  operator unsigned long() const { return as<long>(); }
  operator unsigned short() const { return as<long>(); }
  operator const char *() const;
  operator JsonArray &() const;
  operator JsonObject &() const;

  const char *asString() const { return this->as<const char *>(); }
  JsonArray &asArray() const { return this->as<JsonArray &>(); };
  JsonObject &asObject() const { return this->as<JsonObject &>(); };

  template <typename T>
  T as() const {
    return static_cast<T>(*this);
  }

  template <typename T>
  bool is() const {
    return false;
  }

  static JsonValue &invalid() { return _invalid; }

  bool success() { return _type != Internals::JSON_INVALID; }

  template <typename T>
  void writeTo(T &writer) const;

 private:
  JsonValue(Internals::JsonValueType type) : _type(type) {}

  Internals::JsonValueType _type;
  Internals::JsonValueContent _content;
  static JsonValue _invalid;
};

template <>
inline bool JsonValue::is<long>() const {
  return _type == Internals::JSON_LONG;
}

template <>
inline bool JsonValue::is<double>() const {
  return _type >= Internals::JSON_DOUBLE_0_DECIMALS;
}

template <typename T>
inline bool operator==(const JsonValue &left, T right) {
  return left.as<T>() == right;
}

template <typename T>
inline bool operator==(T left, const JsonValue &right) {
  return left == right.as<T>();
}

template <typename T>
inline bool operator!=(const JsonValue &left, T right) {
  return left.as<T>() != right;
}

template <typename T>
inline bool operator!=(T left, const JsonValue &right) {
  return left != right.as<T>();
}

template <typename T>
inline bool operator<=(const JsonValue &left, T right) {
  return left.as<T>() <= right;
}

template <typename T>
inline bool operator<=(T left, const JsonValue &right) {
  return left <= right.as<T>();
}

template <typename T>
inline bool operator>=(const JsonValue &left, T right) {
  return left.as<T>() >= right;
}

template <typename T>
inline bool operator>=(T left, const JsonValue &right) {
  return left >= right.as<T>();
}

template <typename T>
inline bool operator<(const JsonValue &left, T right) {
  return left.as<T>() < right;
}

template <typename T>
inline bool operator<(T left, const JsonValue &right) {
  return left < right.as<T>();
}

template <typename T>
inline bool operator>(const JsonValue &left, T right) {
  return left.as<T>() > right;
}

template <typename T>
inline bool operator>(T left, const JsonValue &right) {
  return left > right.as<T>();
}
}
