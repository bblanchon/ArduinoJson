// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "JsonVariant.hpp"

namespace ArduinoJson {

inline JsonVariant::JsonVariant(bool value) {
  using namespace Internals;
  _type = JSON_BOOLEAN;
  _content.asInteger = static_cast<JsonInteger>(value);
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
  using namespace Internals;
  _type = static_cast<JsonVariantType>(JSON_FLOAT_0_DECIMALS + decimals);
  _content.asFloat = static_cast<JsonFloat>(value);
}

inline JsonVariant::JsonVariant(float value, uint8_t decimals) {
  using namespace Internals;
  _type = static_cast<JsonVariantType>(JSON_FLOAT_0_DECIMALS + decimals);
  _content.asFloat = static_cast<JsonFloat>(value);
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
  using namespace Internals;
  _type = JSON_INTEGER;
  _content.asInteger = static_cast<JsonInteger>(value);
}

inline JsonVariant::JsonVariant(signed int value) {
  using namespace Internals;
  _type = JSON_INTEGER;
  _content.asInteger = static_cast<JsonInteger>(value);
}

inline JsonVariant::JsonVariant(signed long value) {
  using namespace Internals;
  _type = JSON_INTEGER;
  _content.asInteger = static_cast<JsonInteger>(value);
}

inline JsonVariant::JsonVariant(signed short value) {
  using namespace Internals;
  _type = JSON_INTEGER;
  _content.asInteger = static_cast<JsonInteger>(value);
}

inline JsonVariant::JsonVariant(unsigned char value) {
  using namespace Internals;
  _type = JSON_INTEGER;
  _content.asInteger = static_cast<JsonInteger>(value);
}

inline JsonVariant::JsonVariant(unsigned int value) {
  using namespace Internals;
  _type = JSON_INTEGER;
  _content.asInteger = static_cast<JsonInteger>(value);
}

inline JsonVariant::JsonVariant(unsigned long value) {
  using namespace Internals;
  _type = JSON_INTEGER;
  _content.asInteger = static_cast<JsonInteger>(value);
}

inline JsonVariant::JsonVariant(unsigned short value) {
  using namespace Internals;
  _type = JSON_INTEGER;
  _content.asInteger = static_cast<JsonInteger>(value);
}

template <>
String JsonVariant::as<String>() const;

template <>
const char *JsonVariant::as<const char *>() const;

template <>
inline bool JsonVariant::as<bool>() const {
  return asInteger() != 0;
}

template <>
inline signed char JsonVariant::as<signed char>() const {
  return static_cast<signed char>(asInteger());
}

template <>
inline unsigned char JsonVariant::as<unsigned char>() const {
  return static_cast<unsigned char>(asInteger());
}

template <>
inline signed short JsonVariant::as<signed short>() const {
  return static_cast<signed short>(asInteger());
}

template <>
inline unsigned short JsonVariant::as<unsigned short>() const {
  return static_cast<unsigned short>(asInteger());
}

template <>
inline signed int JsonVariant::as<signed int>() const {
  return static_cast<signed int>(asInteger());
}

template <>
inline unsigned int JsonVariant::as<unsigned int>() const {
  return static_cast<unsigned int>(asInteger());
}

template <>
inline unsigned long JsonVariant::as<unsigned long>() const {
  return static_cast<unsigned long>(asInteger());
}

template <>
inline signed long JsonVariant::as<signed long>() const {
  return static_cast<unsigned long>(asInteger());
}

template <>
inline double JsonVariant::as<double>() const {
  return static_cast<double>(asFloat());
}

template <>
inline float JsonVariant::as<float>() const {
  return static_cast<float>(asFloat());
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

template <>  // int .cpp
bool JsonVariant::is<bool>() const;

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
