// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "JsonVariantBase.hpp"

namespace ArduinoJson {

template <typename TImpl>
class JsonSubscriptBase : public JsonVariantBase<TImpl> {
 public:
  FORCE_INLINE TImpl& operator=(bool value) { return assign<bool>(value); }

  FORCE_INLINE TImpl& operator=(float value) { return assign<float>(value); }

  FORCE_INLINE TImpl& operator=(double value) { return assign<double>(value); }

  FORCE_INLINE TImpl& operator=(signed char value) {
    return assign<signed char>(value);
  }

  FORCE_INLINE TImpl& operator=(signed long value) {
    return assign<signed long>(value);
  }

  FORCE_INLINE TImpl& operator=(signed int value) {
    return assign<signed int>(value);
  }

  FORCE_INLINE TImpl& operator=(signed short value) {
    return assign<signed short>(value);
  }

  FORCE_INLINE TImpl& operator=(unsigned char value) {
    return assign<unsigned char>(value);
  }

  FORCE_INLINE TImpl& operator=(unsigned long value) {
    return assign<unsigned long>(value);
  }

  FORCE_INLINE TImpl& operator=(unsigned int value) {
    return assign<unsigned int>(value);
  }

  FORCE_INLINE TImpl& operator=(unsigned short value) {
    return assign<unsigned short>(value);
  }

  FORCE_INLINE TImpl& operator=(const char* value) {
    return assign<const char*>(value);
  }

  FORCE_INLINE TImpl& operator=(const String& value) {
    return assign<const String&>(value);
  }

  FORCE_INLINE TImpl& operator=(JsonArray& array) {
    return assign<JsonArray&>(array);
  }

  FORCE_INLINE TImpl& operator=(JsonObject& object) {
    return assign<JsonObject&>(object);
  }

  FORCE_INLINE TImpl& operator=(JsonVariant value) {
    return assign<const JsonVariant&>(value);
  }

 protected:
  template <typename TValue>
  FORCE_INLINE TImpl& assign(TValue value) {
    TImpl* that = static_cast<TImpl*>(this);
    that->template set<TValue>(value);
    return *that;
  }
};
}
