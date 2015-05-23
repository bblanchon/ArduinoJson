// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

namespace ArduinoJson {

// Forward declarations
class JsonArray;
class JsonObject;

namespace Internals {
template <>
inline bool JsonVariantContent::as<bool>() const {
  return asBoolean;
}

template <>
inline char const* JsonVariantContent::as<char const*>() const {
  return asString;
}

template <>
inline double JsonVariantContent::as<double>() const {
  return asDouble;
}

template <>
inline float JsonVariantContent::as<float>() const {
  return static_cast<float>(asDouble);
}

template <>
inline JsonArray& JsonVariantContent::as<JsonArray&>() const {
  return *asArray;
}

template <>
inline const JsonArray& JsonVariantContent::as<JsonArray const&>() const {
  return *asArray;
}

template <>
inline JsonObject& JsonVariantContent::as<JsonObject&>() const {
  return *asObject;
}

template <>
inline const JsonObject& JsonVariantContent::as<JsonObject const&>() const {
  return *asObject;
}

template <>
inline signed char JsonVariantContent::as<signed char>() const {
  return static_cast<signed char>(asLong);
}

template <>
inline signed int JsonVariantContent::as<signed int>() const {
  return static_cast<signed int>(asLong);
}

template <>
inline signed long JsonVariantContent::as<signed long>() const {
  return static_cast<signed long>(asLong);
}

template <>
inline signed short JsonVariantContent::as<signed short>() const {
  return static_cast<signed short>(asLong);
}

template <>
inline unsigned char JsonVariantContent::as<unsigned char>() const {
  return static_cast<unsigned char>(asLong);
}

template <>
inline unsigned int JsonVariantContent::as<unsigned int>() const {
  return static_cast<unsigned int>(asLong);
}

template <>
inline unsigned long JsonVariantContent::as<unsigned long>() const {
  return static_cast<unsigned long>(asLong);
}

template <>
inline unsigned short JsonVariantContent::as<unsigned short>() const {
  return static_cast<unsigned short>(asLong);
}
}
}
