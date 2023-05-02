// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Misc/SerializedValue.hpp>
#include <ArduinoJson/Numbers/convertNumber.hpp>
#include <ArduinoJson/Strings/JsonString.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <ArduinoJson/Variant/VariantContent.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class VariantData {
  VariantContent content_;  // must be first to allow cast from array to variant
  uint8_t flags_;

 public:
  VariantData() : flags_(VALUE_IS_NULL) {}

  void reset() {
    flags_ = VALUE_IS_NULL;
  }

  void operator=(const VariantData& src) {
    content_ = src.content_;
    flags_ = uint8_t((flags_ & OWNED_KEY_BIT) | (src.flags_ & ~OWNED_KEY_BIT));
  }

  template <typename TVisitor>
  typename TVisitor::result_type accept(TVisitor& visitor) const {
    switch (type()) {
      case VALUE_IS_FLOAT:
        return visitor.visitFloat(content_.asFloat);

      case VALUE_IS_ARRAY:
        return visitor.visitArray(content_.asCollection);

      case VALUE_IS_OBJECT:
        return visitor.visitObject(content_.asCollection);

      case VALUE_IS_LINKED_STRING:
        return visitor.visitString(content_.asLinkedString,
                                   strlen(content_.asLinkedString));

      case VALUE_IS_OWNED_STRING:
        return visitor.visitString(content_.asOwnedString->data,
                                   content_.asOwnedString->length);

      case VALUE_IS_RAW_STRING:
        return visitor.visitRawString(content_.asOwnedString->data,
                                      content_.asOwnedString->length);

      case VALUE_IS_SIGNED_INTEGER:
        return visitor.visitSignedInteger(content_.asSignedInteger);

      case VALUE_IS_UNSIGNED_INTEGER:
        return visitor.visitUnsignedInteger(content_.asUnsignedInteger);

      case VALUE_IS_BOOLEAN:
        return visitor.visitBoolean(content_.asBoolean != 0);

      default:
        return visitor.visitNull();
    }
  }

  template <typename T>
  T asIntegral() const;

  template <typename T>
  T asFloat() const;

  JsonString asString() const;
  JsonString asRawString() const;

  bool asBoolean() const;

  const char* getOwnedString() const {
    if (flags_ & OWNED_VALUE_BIT)
      return content_.asOwnedString->data;
    else
      return nullptr;
  }

  CollectionData* asArray() {
    return isArray() ? &content_.asCollection : 0;
  }

  const CollectionData* asArray() const {
    return const_cast<VariantData*>(this)->asArray();
  }

  const CollectionData* asCollection() const {
    return isCollection() ? &content_.asCollection : 0;
  }

  CollectionData* asObject() {
    return isObject() ? &content_.asCollection : 0;
  }

  const CollectionData* asObject() const {
    return const_cast<VariantData*>(this)->asObject();
  }

  bool isArray() const {
    return (flags_ & VALUE_IS_ARRAY) != 0;
  }

  bool isBoolean() const {
    return type() == VALUE_IS_BOOLEAN;
  }

  bool isCollection() const {
    return (flags_ & COLLECTION_MASK) != 0;
  }

  template <typename T>
  bool isInteger() const {
    switch (type()) {
      case VALUE_IS_UNSIGNED_INTEGER:
        return canConvertNumber<T>(content_.asUnsignedInteger);

      case VALUE_IS_SIGNED_INTEGER:
        return canConvertNumber<T>(content_.asSignedInteger);

      default:
        return false;
    }
  }

  bool isFloat() const {
    return (flags_ & NUMBER_BIT) != 0;
  }

  bool isString() const {
    return type() == VALUE_IS_LINKED_STRING || type() == VALUE_IS_OWNED_STRING;
  }

  bool isObject() const {
    return (flags_ & VALUE_IS_OBJECT) != 0;
  }

  bool isNull() const {
    return type() == VALUE_IS_NULL;
  }

  bool isEnclosed() const {
    return !isFloat();
  }

  void setBoolean(bool value) {
    setType(VALUE_IS_BOOLEAN);
    content_.asBoolean = value;
  }

  void setFloat(JsonFloat value) {
    setType(VALUE_IS_FLOAT);
    content_.asFloat = value;
  }

  void setRawString(StringNode* s) {
    ARDUINOJSON_ASSERT(s);
    setType(VALUE_IS_RAW_STRING);
    content_.asOwnedString = s;
  }

  template <typename T>
  typename enable_if<is_unsigned<T>::value>::type setInteger(T value) {
    setType(VALUE_IS_UNSIGNED_INTEGER);
    content_.asUnsignedInteger = static_cast<JsonUInt>(value);
  }

  template <typename T>
  typename enable_if<is_signed<T>::value>::type setInteger(T value) {
    setType(VALUE_IS_SIGNED_INTEGER);
    content_.asSignedInteger = value;
  }

  void setNull() {
    setType(VALUE_IS_NULL);
  }

  void setString(StringNode* s) {
    ARDUINOJSON_ASSERT(s);
    setType(VALUE_IS_OWNED_STRING);
    content_.asOwnedString = s;
  }

  void setString(const char* s) {
    ARDUINOJSON_ASSERT(s);
    setType(VALUE_IS_LINKED_STRING);
    content_.asLinkedString = s;
  }

  CollectionData& toArray() {
    setType(VALUE_IS_ARRAY);
    content_.asCollection.clear();
    return content_.asCollection;
  }

  CollectionData& toObject() {
    setType(VALUE_IS_OBJECT);
    content_.asCollection.clear();
    return content_.asCollection;
  }

  size_t memoryUsage() const {
    switch (type()) {
      case VALUE_IS_OWNED_STRING:
      case VALUE_IS_RAW_STRING:
        return sizeofString(content_.asOwnedString->length);
      case VALUE_IS_OBJECT:
      case VALUE_IS_ARRAY:
        return content_.asCollection.memoryUsage();
      default:
        return 0;
    }
  }

  size_t size() const {
    return isCollection() ? content_.asCollection.size() : 0;
  }

  VariantData* getElement(size_t index) const {
    auto array = asArray();
    if (!array)
      return nullptr;
    return slotData(array->get(index));
  }

  template <typename TAdaptedString>
  VariantData* getMember(TAdaptedString key) const {
    auto object = asObject();
    if (!object)
      return nullptr;
    return slotData(object->get(key));
  }

  void movePointers(ptrdiff_t variantDistance) {
    if (flags_ & COLLECTION_MASK)
      content_.asCollection.movePointers(variantDistance);
  }

  uint8_t type() const {
    return flags_ & VALUE_MASK;
  }

 private:
  void setType(uint8_t t) {
    flags_ &= OWNED_KEY_BIT;
    flags_ |= t;
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
