// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/MemoryPool.hpp>
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
      case VALUE_IS_OWNED_STRING:
        return visitor.visitString(content_.asString.data,
                                   content_.asString.size);

      case VALUE_IS_OWNED_RAW:
      case VALUE_IS_LINKED_RAW:
        return visitor.visitRawJson(content_.asString.data,
                                    content_.asString.size);

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

  bool asBoolean() const;

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

  bool copyFrom(const VariantData& src, MemoryPool* pool);

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

  void remove(size_t index) {
    if (isArray())
      content_.asCollection.removeElement(index);
  }

  template <typename TAdaptedString>
  void remove(TAdaptedString key) {
    if (isObject())
      content_.asCollection.removeMember(key);
  }

  void setBoolean(bool value) {
    setType(VALUE_IS_BOOLEAN);
    content_.asBoolean = value;
  }

  void setFloat(JsonFloat value) {
    setType(VALUE_IS_FLOAT);
    content_.asFloat = value;
  }

  void setLinkedRaw(SerializedValue<const char*> value) {
    if (value.data()) {
      setType(VALUE_IS_LINKED_RAW);
      content_.asString.data = value.data();
      content_.asString.size = value.size();
    } else {
      setType(VALUE_IS_NULL);
    }
  }

  template <typename T>
  bool storeOwnedRaw(SerializedValue<T> value, MemoryPool* pool) {
    const char* dup = pool->saveString(adaptString(value.data(), value.size()));
    if (dup) {
      setType(VALUE_IS_OWNED_RAW);
      content_.asString.data = dup;
      content_.asString.size = value.size();
      return true;
    } else {
      setType(VALUE_IS_NULL);
      return false;
    }
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

  void setString(JsonString s) {
    ARDUINOJSON_ASSERT(s);
    if (s.isLinked())
      setType(VALUE_IS_LINKED_STRING);
    else
      setType(VALUE_IS_OWNED_STRING);
    content_.asString.data = s.c_str();
    content_.asString.size = s.size();
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
      case VALUE_IS_OWNED_RAW:
        // We always add a zero at the end: the deduplication function uses it
        // to detect the beginning of the next string.
        return content_.asString.size + 1;
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

  VariantData* addElement(MemoryPool* pool) {
    if (isNull())
      toArray();
    if (!isArray())
      return 0;
    return content_.asCollection.addElement(pool);
  }

  VariantData* getElement(size_t index) const {
    const CollectionData* col = asArray();
    return col ? col->getElement(index) : 0;
  }

  VariantData* getOrAddElement(size_t index, MemoryPool* pool) {
    if (isNull())
      toArray();
    if (!isArray())
      return 0;
    return content_.asCollection.getOrAddElement(index, pool);
  }

  template <typename TAdaptedString>
  VariantData* getMember(TAdaptedString key) const {
    const CollectionData* col = asObject();
    return col ? col->getMember(key) : 0;
  }

  template <typename TAdaptedString>
  VariantData* getOrAddMember(TAdaptedString key, MemoryPool* pool) {
    if (isNull())
      toObject();
    if (!isObject())
      return 0;
    return content_.asCollection.getOrAddMember(key, pool);
  }

  void movePointers(ptrdiff_t stringDistance, ptrdiff_t variantDistance) {
    if (flags_ & OWNED_VALUE_BIT)
      content_.asString.data += stringDistance;
    if (flags_ & COLLECTION_MASK)
      content_.asCollection.movePointers(stringDistance, variantDistance);
  }

  uint8_t type() const {
    return flags_ & VALUE_MASK;
  }

  template <typename TAdaptedString>
  inline bool setString(TAdaptedString value, MemoryPool* pool) {
    if (value.isNull()) {
      setNull();
      return true;
    }

    return storeString(pool, value, VariantStringSetter(this));
  }

 private:
  void setType(uint8_t t) {
    flags_ &= OWNED_KEY_BIT;
    flags_ |= t;
  }

  struct VariantStringSetter {
    VariantStringSetter(VariantData* instance) : instance_(instance) {}

    template <typename TStoredString>
    void operator()(TStoredString s) {
      if (s)
        instance_->setString(s);
      else
        instance_->setNull();
    }

    VariantData* instance_;
  };
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
