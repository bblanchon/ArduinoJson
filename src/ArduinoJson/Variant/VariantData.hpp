// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/StringNode.hpp>
#include <ArduinoJson/Misc/SerializedValue.hpp>
#include <ArduinoJson/Numbers/convertNumber.hpp>
#include <ArduinoJson/Strings/JsonString.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <ArduinoJson/Variant/VariantContent.hpp>
#include <ArduinoJson/Variant/VariantSlot.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

VariantData* collectionAddElement(CollectionData* array, MemoryPool* pool);
void collectionRemoveElement(CollectionData* data, size_t index,
                             MemoryPool* pool);
template <typename T>
T parseNumber(const char* s);
void slotRelease(VariantSlot* slot, MemoryPool* pool);

class VariantData {
  VariantContent content_;  // must be first to allow cast from array to variant
  uint8_t flags_;

 public:
  VariantData() : flags_(VALUE_IS_NULL) {}

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

  VariantData* addElement(MemoryPool* pool) {
    auto array = isNull() ? &toArray() : asArray();
    return collectionAddElement(array, pool);
  }

  bool asBoolean() const {
    switch (type()) {
      case VALUE_IS_BOOLEAN:
        return content_.asBoolean;
      case VALUE_IS_SIGNED_INTEGER:
      case VALUE_IS_UNSIGNED_INTEGER:
        return content_.asUnsignedInteger != 0;
      case VALUE_IS_FLOAT:
        return content_.asFloat != 0;
      case VALUE_IS_NULL:
        return false;
      default:
        return true;
    }
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

  template <typename T>
  T asFloat() const {
    static_assert(is_floating_point<T>::value, "T must be a floating point");
    switch (type()) {
      case VALUE_IS_BOOLEAN:
        return static_cast<T>(content_.asBoolean);
      case VALUE_IS_UNSIGNED_INTEGER:
        return static_cast<T>(content_.asUnsignedInteger);
      case VALUE_IS_SIGNED_INTEGER:
        return static_cast<T>(content_.asSignedInteger);
      case VALUE_IS_LINKED_STRING:
      case VALUE_IS_OWNED_STRING:
        return parseNumber<T>(content_.asOwnedString->data);
      case VALUE_IS_FLOAT:
        return static_cast<T>(content_.asFloat);
      default:
        return 0;
    }
  }

  template <typename T>
  T asIntegral() const {
    static_assert(is_integral<T>::value, "T must be an integral type");
    switch (type()) {
      case VALUE_IS_BOOLEAN:
        return content_.asBoolean;
      case VALUE_IS_UNSIGNED_INTEGER:
        return convertNumber<T>(content_.asUnsignedInteger);
      case VALUE_IS_SIGNED_INTEGER:
        return convertNumber<T>(content_.asSignedInteger);
      case VALUE_IS_LINKED_STRING:
        return parseNumber<T>(content_.asLinkedString);
      case VALUE_IS_OWNED_STRING:
        return parseNumber<T>(content_.asOwnedString->data);
      case VALUE_IS_FLOAT:
        return convertNumber<T>(content_.asFloat);
      default:
        return 0;
    }
  }

  CollectionData* asObject() {
    return isObject() ? &content_.asCollection : 0;
  }

  const CollectionData* asObject() const {
    return const_cast<VariantData*>(this)->asObject();
  }

  JsonString asRawString() const {
    switch (type()) {
      case VALUE_IS_RAW_STRING:
        return JsonString(content_.asOwnedString->data,
                          content_.asOwnedString->length, JsonString::Copied);
      default:
        return JsonString();
    }
  }

  JsonString asString() const {
    switch (type()) {
      case VALUE_IS_LINKED_STRING:
        return JsonString(content_.asLinkedString, JsonString::Linked);
      case VALUE_IS_OWNED_STRING:
        return JsonString(content_.asOwnedString->data,
                          content_.asOwnedString->length, JsonString::Copied);
      default:
        return JsonString();
    }
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

  VariantData* getOrAddElement(size_t index, MemoryPool* pool) {
    auto array = isNull() ? &toArray() : asArray();
    if (!array)
      return nullptr;
    VariantSlot* slot = array->head();
    while (slot && index > 0) {
      slot = slot->next();
      index--;
    }
    if (!slot)
      index++;
    while (index > 0) {
      slot = new (pool) VariantSlot();
      if (!slot)
        return nullptr;
      array->add(slot);
      index--;
    }
    return slot->data();
  }

  template <typename TAdaptedString>
  VariantData* getOrAddMember(TAdaptedString key, MemoryPool* pool) {
    if (key.isNull())
      return nullptr;
    auto obj = isNull() ? &toObject() : asObject();
    if (!obj)
      return nullptr;
    auto slot = obj->get(key);
    if (slot)
      return slot->data();
    return collectionAddMember(obj, key, pool);
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

  bool isFloat() const {
    return (flags_ & NUMBER_BIT) != 0;
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

  bool isNull() const {
    return type() == VALUE_IS_NULL;
  }

  bool isObject() const {
    return (flags_ & VALUE_IS_OBJECT) != 0;
  }

  bool isString() const {
    return type() == VALUE_IS_LINKED_STRING || type() == VALUE_IS_OWNED_STRING;
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

  void movePointers(ptrdiff_t variantDistance) {
    if (flags_ & COLLECTION_MASK)
      content_.asCollection.movePointers(variantDistance);
  }

  size_t nesting() const {
    auto collection = asCollection();
    if (!collection)
      return 0;

    size_t maxChildNesting = 0;
    for (const VariantSlot* s = collection->head(); s; s = s->next()) {
      size_t childNesting = s->data()->nesting();
      if (childNesting > maxChildNesting)
        maxChildNesting = childNesting;
    }
    return maxChildNesting + 1;
  }

  void operator=(const VariantData& src) {
    content_ = src.content_;
    flags_ = uint8_t((flags_ & OWNED_KEY_BIT) | (src.flags_ & ~OWNED_KEY_BIT));
  }

  void removeElement(size_t index, MemoryPool* pool) {
    collectionRemoveElement(asArray(), index, pool);
  }

  template <typename TAdaptedString>
  void removeMember(TAdaptedString key, MemoryPool* pool) {
    collectionRemoveMember(asObject(), key, pool);
  }

  void reset() {
    flags_ = VALUE_IS_NULL;
  }

  void setBoolean(bool value) {
    setType(VALUE_IS_BOOLEAN);
    content_.asBoolean = value;
  }

  void setBoolean(bool value, MemoryPool* pool) {
    release(pool);
    setBoolean(value);
  }

  void setFloat(JsonFloat value) {
    setType(VALUE_IS_FLOAT);
    content_.asFloat = value;
  }

  void setFloat(JsonFloat value, MemoryPool* pool) {
    release(pool);
    setFloat(value);
  }

  template <typename T>
  typename enable_if<is_signed<T>::value>::type setInteger(T value) {
    setType(VALUE_IS_SIGNED_INTEGER);
    content_.asSignedInteger = value;
  }

  template <typename T>
  typename enable_if<is_unsigned<T>::value>::type setInteger(T value) {
    setType(VALUE_IS_UNSIGNED_INTEGER);
    content_.asUnsignedInteger = static_cast<JsonUInt>(value);
  }

  template <typename T>
  void setInteger(T value, MemoryPool* pool) {
    release(pool);
    setInteger(value);
  }

  void setNull() {
    setType(VALUE_IS_NULL);
  }

  void setNull(MemoryPool* pool) {
    release(pool);
    setNull();
  }

  void setRawString(StringNode* s) {
    ARDUINOJSON_ASSERT(s);
    setType(VALUE_IS_RAW_STRING);
    content_.asOwnedString = s;
  }

  template <typename T>
  void setRawString(SerializedValue<T> value, MemoryPool* pool) {
    release(pool);
    auto dup = pool->saveString(adaptString(value.data(), value.size()));
    if (dup)
      setRawString(dup);
    else
      setNull();
  }

  template <typename TAdaptedString>
  void setString(TAdaptedString value, MemoryPool* pool) {
    setNull(pool);

    if (value.isNull())
      return;

    if (value.isLinked()) {
      setLinkedString(value.data());
      return;
    }

    auto dup = pool->saveString(value);
    if (dup)
      setOwnedString(dup);
  }

  void setLinkedString(const char* s) {
    ARDUINOJSON_ASSERT(s);
    setType(VALUE_IS_LINKED_STRING);
    content_.asLinkedString = s;
  }

  void setOwnedString(StringNode* s) {
    ARDUINOJSON_ASSERT(s);
    setType(VALUE_IS_OWNED_STRING);
    content_.asOwnedString = s;
  }

  size_t size() const {
    return isCollection() ? content_.asCollection.size() : 0;
  }

  CollectionData& toArray() {
    setType(VALUE_IS_ARRAY);
    content_.asCollection.clear();
    return content_.asCollection;
  }

  CollectionData& toArray(MemoryPool* pool) {
    release(pool);
    return toArray();
  }

  CollectionData& toObject() {
    setType(VALUE_IS_OBJECT);
    content_.asCollection.clear();
    return content_.asCollection;
  }

  CollectionData& toObject(MemoryPool* pool) {
    release(pool);
    return toObject();
  }

  uint8_t type() const {
    return flags_ & VALUE_MASK;
  }

 private:
  void release(MemoryPool* pool) {
    if (flags_ & OWNED_VALUE_BIT)
      pool->dereferenceString(content_.asOwnedString->data);

    auto c = asCollection();
    if (c) {
      for (auto slot = c->head(); slot; slot = slot->next())
        slotRelease(slot, pool);
    }
  }

  void setType(uint8_t t) {
    flags_ &= OWNED_KEY_BIT;
    flags_ |= t;
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
