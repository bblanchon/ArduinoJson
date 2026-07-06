// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/StringNode.hpp>
#include <ArduinoJson/Strings/JsonString.hpp>
#include <ArduinoJson/Variant/VariantContent.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename T>
T parseNumber(const char* s);

template <typename T>
static bool isTinyString(const T& s, size_t n) {
  if (n > tinyStringMaxLength)
    return false;
  bool containsNul = false;
  for (uint8_t i = 0; i < uint8_t(n); i++)
    containsNul |= !s[i];
  return !containsNul;
}

struct VariantData {
  VariantContent content;  // must be first to allow cast from array to variant
  VariantType type = VariantType::Null;
  SlotId next = NULL_SLOT;

  // Placement new
  static void* operator new(size_t, void* p) noexcept {
    return p;
  }

  static void operator delete(void*, void*) noexcept {}

  JsonString asRawString() const {
    switch (type) {
      case VariantType::RawString:
        return JsonString(content.asStringNode->data,
                          content.asStringNode->length);
      default:
        return JsonString();
    }
  }

  JsonString asString() const {
    switch (type) {
      case VariantType::TinyString:
        return JsonString(content.asTinyString);
      case VariantType::LongString:
        return JsonString(content.asStringNode->data,
                          content.asStringNode->length);
      default:
        return JsonString();
    }
  }

  bool isArray() const {
    return type == VariantType::Array;
  }

  bool isBoolean() const {
    return type == VariantType::Boolean;
  }

  bool isCollection() const {
    return type & VariantTypeBits::CollectionMask;
  }

  bool isFloat() const {
    return type & VariantTypeBits::NumberBit;
  }

  bool isNull() const {
    return type == VariantType::Null;
  }

  bool isObject() const {
    return type == VariantType::Object;
  }

  bool isString() const {
    return type == VariantType::LongString || type == VariantType::TinyString;
  }

  void setBoolean(bool value) {
    ARDUINOJSON_ASSERT(type == VariantType::Null);
    type = VariantType::Boolean;
    content.asBoolean = value;
  }

  void setRawString(StringNode* s) {
    ARDUINOJSON_ASSERT(type == VariantType::Null);
    ARDUINOJSON_ASSERT(s);
    type = VariantType::RawString;
    content.asStringNode = s;
  }

  template <typename TAdaptedString>
  void setTinyString(const TAdaptedString& s) {
    ARDUINOJSON_ASSERT(type == VariantType::Null);
    ARDUINOJSON_ASSERT(s.size() <= tinyStringMaxLength);

    type = VariantType::TinyString;

    auto n = uint8_t(s.size());
    for (uint8_t i = 0; i < n; i++) {
      char c = s[i];
      ARDUINOJSON_ASSERT(c != 0);  // no NUL in tiny string
      content.asTinyString[i] = c;
    }

    content.asTinyString[n] = 0;
  }

  void setLongString(StringNode* s) {
    ARDUINOJSON_ASSERT(type == VariantType::Null);
    ARDUINOJSON_ASSERT(s);
    type = VariantType::LongString;
    content.asStringNode = s;
  }

  CollectionData* toArray() {
    ARDUINOJSON_ASSERT(type == VariantType::Null);
    type = VariantType::Array;
    return new (&content.asCollection) CollectionData();
  }

  CollectionData* toObject() {
    ARDUINOJSON_ASSERT(type == VariantType::Null);
    type = VariantType::Object;
    return new (&content.asCollection) CollectionData();
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
