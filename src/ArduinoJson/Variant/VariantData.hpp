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
  VariantContent _content;  // must be first to allow cast from array to variant
  uint8_t _flags;

 public:
  VariantData() : _flags(VALUE_IS_NULL) {}

  void reset() {
    _flags = VALUE_IS_NULL;
  }

  void operator=(const VariantData& src) {
    _content = src._content;
    _flags = uint8_t((_flags & OWNED_KEY_BIT) | (src._flags & ~OWNED_KEY_BIT));
  }

  template <typename TVisitor>
  typename TVisitor::result_type accept(TVisitor& visitor) const {
    switch (type()) {
      case VALUE_IS_FLOAT:
        return visitor.visitFloat(_content.asFloat);

      case VALUE_IS_ARRAY:
        return visitor.visitArray(_content.asCollection);

      case VALUE_IS_OBJECT:
        return visitor.visitObject(_content.asCollection);

      case VALUE_IS_LINKED_STRING:
      case VALUE_IS_OWNED_STRING:
        return visitor.visitString(_content.asString.data,
                                   _content.asString.size);

      case VALUE_IS_OWNED_RAW:
      case VALUE_IS_LINKED_RAW:
        return visitor.visitRawJson(_content.asString.data,
                                    _content.asString.size);

      case VALUE_IS_SIGNED_INTEGER:
        return visitor.visitSignedInteger(_content.asSignedInteger);

      case VALUE_IS_UNSIGNED_INTEGER:
        return visitor.visitUnsignedInteger(_content.asUnsignedInteger);

      case VALUE_IS_BOOLEAN:
        return visitor.visitBoolean(_content.asBoolean != 0);

      default:
        return visitor.visitNull();
    }
  }

  template <typename T>
  T asIntegral() const;

  template <typename T>
  T asFloat() const;

  JsonString asString() const;
  JsonString asRaw() const;

  bool asBoolean() const;

  const char* getOwnedString() const {
    if (_flags & OWNED_VALUE_BIT)
      return _content.asString.data;
    else
      return nullptr;
  }

  CollectionData* asArray() {
    return isArray() ? &_content.asCollection : 0;
  }

  const CollectionData* asArray() const {
    return const_cast<VariantData*>(this)->asArray();
  }

  const CollectionData* asCollection() const {
    return isCollection() ? &_content.asCollection : 0;
  }

  CollectionData* asObject() {
    return isObject() ? &_content.asCollection : 0;
  }

  const CollectionData* asObject() const {
    return const_cast<VariantData*>(this)->asObject();
  }

  bool isArray() const {
    return (_flags & VALUE_IS_ARRAY) != 0;
  }

  bool isBoolean() const {
    return type() == VALUE_IS_BOOLEAN;
  }

  bool isCollection() const {
    return (_flags & COLLECTION_MASK) != 0;
  }

  template <typename T>
  bool isInteger() const {
    switch (type()) {
      case VALUE_IS_UNSIGNED_INTEGER:
        return canConvertNumber<T>(_content.asUnsignedInteger);

      case VALUE_IS_SIGNED_INTEGER:
        return canConvertNumber<T>(_content.asSignedInteger);

      default:
        return false;
    }
  }

  bool isFloat() const {
    return (_flags & NUMBER_BIT) != 0;
  }

  bool isString() const {
    return type() == VALUE_IS_LINKED_STRING || type() == VALUE_IS_OWNED_STRING;
  }

  bool isObject() const {
    return (_flags & VALUE_IS_OBJECT) != 0;
  }

  bool isNull() const {
    return type() == VALUE_IS_NULL;
  }

  bool isEnclosed() const {
    return !isFloat();
  }

  void setBoolean(bool value) {
    setType(VALUE_IS_BOOLEAN);
    _content.asBoolean = value;
  }

  void setFloat(JsonFloat value) {
    setType(VALUE_IS_FLOAT);
    _content.asFloat = value;
  }

  void setLinkedRaw(const char* data, size_t n) {
    setType(VALUE_IS_LINKED_RAW);
    _content.asString.data = data;
    _content.asString.size = n;
  }

  void setOwnedRaw(const char* data, size_t n) {
    setType(VALUE_IS_OWNED_RAW);
    _content.asString.data = data;
    _content.asString.size = n;
  }

  template <typename T>
  typename enable_if<is_unsigned<T>::value>::type setInteger(T value) {
    setType(VALUE_IS_UNSIGNED_INTEGER);
    _content.asUnsignedInteger = static_cast<JsonUInt>(value);
  }

  template <typename T>
  typename enable_if<is_signed<T>::value>::type setInteger(T value) {
    setType(VALUE_IS_SIGNED_INTEGER);
    _content.asSignedInteger = value;
  }

  void setNull() {
    setType(VALUE_IS_NULL);
  }

  void setString(JsonString s) {
    ARDUINOJSON_ASSERT(s);
    if (s.isLinked())
      setType(VALUE_IS_LINKED_STRING);
    else
      setType(VALUE_IS_OWNED_STRING);
    _content.asString.data = s.c_str();
    _content.asString.size = s.size();
  }

  CollectionData& toArray() {
    setType(VALUE_IS_ARRAY);
    _content.asCollection.clear();
    return _content.asCollection;
  }

  CollectionData& toObject() {
    setType(VALUE_IS_OBJECT);
    _content.asCollection.clear();
    return _content.asCollection;
  }

  size_t memoryUsage() const {
    switch (type()) {
      case VALUE_IS_OWNED_STRING:
      case VALUE_IS_OWNED_RAW:
        return sizeofString(_content.asString.size);
      case VALUE_IS_OBJECT:
      case VALUE_IS_ARRAY:
        return _content.asCollection.memoryUsage();
      default:
        return 0;
    }
  }

  size_t size() const {
    return isCollection() ? _content.asCollection.size() : 0;
  }

  VariantData* getElement(size_t index) const {
    const CollectionData* col = asArray();
    return col ? col->getElement(index) : 0;
  }

  template <typename TAdaptedString>
  VariantData* getMember(TAdaptedString key) const {
    const CollectionData* col = asObject();
    return col ? col->getMember(key) : 0;
  }

  void movePointers(ptrdiff_t variantDistance) {
    if (_flags & COLLECTION_MASK)
      _content.asCollection.movePointers(variantDistance);
  }

  uint8_t type() const {
    return _flags & VALUE_MASK;
  }

 private:
  void setType(uint8_t t) {
    _flags &= OWNED_KEY_BIT;
    _flags |= t;
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
