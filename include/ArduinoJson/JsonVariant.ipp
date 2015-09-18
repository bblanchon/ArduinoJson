// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonVariant.hpp"

namespace ArduinoJson {

inline JsonVariant::JsonVariant(bool value) {
  _type = Internals::JSON_BOOLEAN;
  _content.asLong = value;
}

inline JsonVariant::JsonVariant(const char *value) {
  _type = Internals::JSON_STRING;
  _content.asString = value;
}

inline JsonVariant::JsonVariant(Internals::Unparsed value) {
  _type = Internals::JSON_UNPARSED;
  _content.asString = value;
}

inline JsonVariant::JsonVariant(double value, uint8_t decimals) {
  _type = static_cast<Internals::JsonVariantType>(
      Internals::JSON_DOUBLE_0_DECIMALS + decimals);
  _content.asDouble = value;
}

inline JsonVariant::JsonVariant(float value, uint8_t decimals) {
  _type = static_cast<Internals::JsonVariantType>(
      Internals::JSON_DOUBLE_0_DECIMALS + decimals);
  _content.asDouble = value;
}

inline JsonVariant::JsonVariant(JsonArray &array) {
  _type = Internals::JSON_ARRAY;
  _content.asArray = &array;
}

inline JsonVariant::JsonVariant(JsonObject &object) {
  _type = Internals::JSON_OBJECT;
  _content.asObject = &object;
}

inline JsonVariant::JsonVariant(signed char value) {
  _type = Internals::JSON_LONG;
  _content.asLong = value;
}

inline JsonVariant::JsonVariant(signed int value) {
  _type = Internals::JSON_LONG;
  _content.asLong = value;
}

inline JsonVariant::JsonVariant(signed long value) {
  _type = Internals::JSON_LONG;
  _content.asLong = value;
}

inline JsonVariant::JsonVariant(signed short value) {
  _type = Internals::JSON_LONG;
  _content.asLong = value;
}

inline JsonVariant::JsonVariant(unsigned char value) {
  _type = Internals::JSON_LONG;
  _content.asLong = value;
}

inline JsonVariant::JsonVariant(unsigned int value) {
  _type = Internals::JSON_LONG;
  _content.asLong = value;
}

inline JsonVariant::JsonVariant(unsigned long value) {
  _type = Internals::JSON_LONG;
  _content.asLong = value;
}

inline JsonVariant::JsonVariant(unsigned short value) {
  _type = Internals::JSON_LONG;
  _content.asLong = value;
}

template <>
double JsonVariant::as<double>() const;

template <>
long JsonVariant::as<long>() const;

template <>
String JsonVariant::as<String>() const;

template <>
const char *JsonVariant::as<const char *>() const;

template <>
inline bool JsonVariant::as<bool>() const {
  return as<long>() != 0;
}

template <>
inline signed char JsonVariant::as<signed char>() const {
  return static_cast<signed char>(as<long>());
}

template <>
inline unsigned char JsonVariant::as<unsigned char>() const {
  return static_cast<unsigned char>(as<long>());
}

template <>
inline signed short JsonVariant::as<signed short>() const {
  return static_cast<signed short>(as<long>());
}

template <>
inline unsigned short JsonVariant::as<unsigned short>() const {
  return static_cast<unsigned short>(as<long>());
}

template <>
inline signed int JsonVariant::as<signed int>() const {
  return static_cast<signed int>(as<long>());
}

template <>
inline unsigned int JsonVariant::as<unsigned int>() const {
  return static_cast<unsigned int>(as<long>());
}

template <>
inline unsigned long JsonVariant::as<unsigned long>() const {
  return static_cast<unsigned long>(as<long>());
}

template <>
inline float JsonVariant::as<float>() const {
  return static_cast<float>(as<double>());
}

template <typename T>
inline T JsonVariant::invalid() {
  return T();
}

template <typename T>
inline bool JsonVariant::is() const {
  return false;
}

template <>  // in .cpp
bool JsonVariant::is<signed long>() const;

template <>  // in .cpp
bool JsonVariant::is<double>() const;

template <>
inline bool JsonVariant::is<bool>() const {
  return _type == Internals::JSON_BOOLEAN;
}

template <>
inline bool JsonVariant::is<char const *>() const {
  return _type == Internals::JSON_STRING;
}

template <>
inline bool JsonVariant::is<float>() const {
  return is<double>();
}

template <>
inline bool JsonVariant::is<JsonArray &>() const {
  return _type == Internals::JSON_ARRAY;
}

template <>
inline bool JsonVariant::is<JsonArray const &>() const {
  return _type == Internals::JSON_ARRAY;
}

template <>
inline bool JsonVariant::is<JsonObject &>() const {
  return _type == Internals::JSON_OBJECT;
}

template <>
inline bool JsonVariant::is<JsonObject const &>() const {
  return _type == Internals::JSON_OBJECT;
}

template <>
inline bool JsonVariant::is<signed char>() const {
  return is<signed long>();
}

template <>
inline bool JsonVariant::is<signed int>() const {
  return is<signed long>();
}

template <>
inline bool JsonVariant::is<signed short>() const {
  return is<signed long>();
}

template <>
inline bool JsonVariant::is<unsigned char>() const {
  return is<signed long>();
}

template <>
inline bool JsonVariant::is<unsigned int>() const {
  return is<signed long>();
}

template <>
inline bool JsonVariant::is<unsigned long>() const {
  return is<signed long>();
}

template <>
inline bool JsonVariant::is<unsigned short>() const {
  return is<signed long>();
}

#ifdef ARDUINOJSON_ENABLE_STD_STREAM
inline std::ostream &operator<<(std::ostream &os, const JsonVariant &source) {
  return source.printTo(os);
}
#endif

}  // namespace ArduinoJson
