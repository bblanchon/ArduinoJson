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
  _content.asBoolean = value;
}

inline JsonVariant::JsonVariant(const char *value) {
  _type = Internals::JSON_STRING;
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

template <typename T>
inline T JsonVariant::as() const {
  return is<T>() ? _content.as<T>() : invalid<T>();
}

template <typename T>
inline T JsonVariant::invalid() {
  return T();
}

template <typename T>
inline bool JsonVariant::is() const {
  return false;
}

template <>
inline bool JsonVariant::is<bool>() const {
  return _type == Internals::JSON_BOOLEAN;
}

template <>
inline bool JsonVariant::is<char const *>() const {
  return _type == Internals::JSON_STRING;
}

template <>
inline bool JsonVariant::is<double>() const {
  return _type >= Internals::JSON_DOUBLE_0_DECIMALS;
}

template <>
inline bool JsonVariant::is<float>() const {
  return _type >= Internals::JSON_DOUBLE_0_DECIMALS;
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
  return _type == Internals::JSON_LONG;
}

template <>
inline bool JsonVariant::is<signed int>() const {
  return _type == Internals::JSON_LONG;
}

template <>
inline bool JsonVariant::is<signed long>() const {
  return _type == Internals::JSON_LONG;
}

template <>
inline bool JsonVariant::is<signed short>() const {
  return _type == Internals::JSON_LONG;
}

template <>
inline bool JsonVariant::is<unsigned char>() const {
  return _type == Internals::JSON_LONG;
}

template <>
inline bool JsonVariant::is<unsigned int>() const {
  return _type == Internals::JSON_LONG;
}

template <>
inline bool JsonVariant::is<unsigned long>() const {
  return _type == Internals::JSON_LONG;
}

template <>
inline bool JsonVariant::is<unsigned short>() const {
  return _type == Internals::JSON_LONG;
}
}
