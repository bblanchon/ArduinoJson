// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
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

template <typename T>
T parseNumber(const char* s);

class VariantData {
  VariantContent content_;  // must be first to allow cast from array to variant
  uint8_t flags_;

 public:
  VariantData() : flags_(VALUE_IS_NULL) {}

  template <typename TVisitor>
  typename TVisitor::result_type accept(TVisitor& visit) const {
    switch (type()) {
      case VALUE_IS_FLOAT:
        return visit.visit(content_.asFloat);

      case VALUE_IS_ARRAY:
        return visit.visit(content_.asArray);

      case VALUE_IS_OBJECT:
        return visit.visit(content_.asObject);

      case VALUE_IS_LINKED_STRING:
        return visit.visit(JsonString(content_.asLinkedString));

      case VALUE_IS_OWNED_STRING:
        return visit.visit(JsonString(content_.asOwnedString->data,
                                      content_.asOwnedString->length,
                                      JsonString::Copied));

      case VALUE_IS_RAW_STRING:
        return visit.visit(RawString(content_.asOwnedString->data,
                                     content_.asOwnedString->length));

      case VALUE_IS_SIGNED_INTEGER:
        return visit.visit(content_.asSignedInteger);

      case VALUE_IS_UNSIGNED_INTEGER:
        return visit.visit(content_.asUnsignedInteger);

      case VALUE_IS_BOOLEAN:
        return visit.visit(content_.asBoolean != 0);

      default:
        return visit.visit(nullptr);
    }
  }

  template <typename TVisitor>
  static typename TVisitor::result_type accept(const VariantData* var,
                                               TVisitor& visit) {
    if (var != 0)
      return var->accept(visit);
    else
      return visit.visit(nullptr);
  }

  VariantData* addElement(ResourceManager* resources) {
    auto array = isNull() ? &toArray() : asArray();
    return detail::ArrayData::addElement(array, resources);
  }

  static VariantData* addElement(VariantData* var, ResourceManager* resources) {
    if (!var)
      return nullptr;
    return var->addElement(resources);
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

  ArrayData* asArray() {
    return isArray() ? &content_.asArray : 0;
  }

  const ArrayData* asArray() const {
    return const_cast<VariantData*>(this)->asArray();
  }

  CollectionData* asCollection() {
    return isCollection() ? &content_.asCollection : 0;
  }

  const CollectionData* asCollection() const {
    return const_cast<VariantData*>(this)->asCollection();
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

  ObjectData* asObject() {
    return isObject() ? &content_.asObject : 0;
  }

  const ObjectData* asObject() const {
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

  VariantData* getElement(size_t index,
                          const ResourceManager* resources) const {
    return ArrayData::getElement(asArray(), index, resources);
  }

  static VariantData* getElement(const VariantData* var, size_t index,
                                 const ResourceManager* resources) {
    return var != 0 ? var->getElement(index, resources) : 0;
  }

  template <typename TAdaptedString>
  VariantData* getMember(TAdaptedString key,
                         const ResourceManager* resources) const {
    return ObjectData::getMember(asObject(), key, resources);
  }

  template <typename TAdaptedString>
  static VariantData* getMember(const VariantData* var, TAdaptedString key,
                                const ResourceManager* resources) {
    if (!var)
      return 0;
    return var->getMember(key, resources);
  }

  VariantData* getOrAddElement(size_t index, ResourceManager* resources) {
    auto array = isNull() ? &toArray() : asArray();
    if (!array)
      return nullptr;
    return array->getOrAddElement(index, resources);
  }

  template <typename TAdaptedString>
  VariantData* getOrAddMember(TAdaptedString key, ResourceManager* resources) {
    if (key.isNull())
      return nullptr;
    auto obj = isNull() ? &toObject() : asObject();
    if (!obj)
      return nullptr;
    return obj->getOrAddMember(key, resources);
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

  static bool isNull(const VariantData* var) {
    if (!var)
      return true;
    return var->isNull();
  }

  bool isObject() const {
    return (flags_ & VALUE_IS_OBJECT) != 0;
  }

  bool isString() const {
    return type() == VALUE_IS_LINKED_STRING || type() == VALUE_IS_OWNED_STRING;
  }

  size_t nesting(const ResourceManager* resources) const {
    auto collection = asCollection();
    if (collection)
      return collection->nesting(resources);
    else
      return 0;
  }

  static size_t nesting(const VariantData* var,
                        const ResourceManager* resources) {
    if (!var)
      return 0;
    return var->nesting(resources);
  }

  void removeElement(size_t index, ResourceManager* resources) {
    ArrayData::removeElement(asArray(), index, resources);
  }

  static void removeElement(VariantData* var, size_t index,
                            ResourceManager* resources) {
    if (!var)
      return;
    var->removeElement(index, resources);
  }

  template <typename TAdaptedString>
  void removeMember(TAdaptedString key, ResourceManager* resources) {
    ObjectData::removeMember(asObject(), key, resources);
  }

  template <typename TAdaptedString>
  static void removeMember(VariantData* var, TAdaptedString key,
                           ResourceManager* resources) {
    if (!var)
      return;
    var->removeMember(key, resources);
  }

  void reset() {
    flags_ = VALUE_IS_NULL;
  }

  void setBoolean(bool value) {
    setType(VALUE_IS_BOOLEAN);
    content_.asBoolean = value;
  }

  void setBoolean(bool value, ResourceManager* resources) {
    release(resources);
    setBoolean(value);
  }

  void setFloat(JsonFloat value) {
    setType(VALUE_IS_FLOAT);
    content_.asFloat = value;
  }

  void setFloat(JsonFloat value, ResourceManager* resources) {
    release(resources);
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
  void setInteger(T value, ResourceManager* resources) {
    release(resources);
    setInteger(value);
  }

  void setNull() {
    setType(VALUE_IS_NULL);
  }

  void setNull(ResourceManager* resources) {
    release(resources);
    setNull();
  }

  static void setNull(VariantData* var, ResourceManager* resources) {
    if (!var)
      return;
    var->setNull(resources);
  }

  void setRawString(StringNode* s) {
    ARDUINOJSON_ASSERT(s);
    setType(VALUE_IS_RAW_STRING);
    content_.asOwnedString = s;
  }

  template <typename T>
  void setRawString(SerializedValue<T> value, ResourceManager* resources) {
    release(resources);
    auto dup = resources->saveString(adaptString(value.data(), value.size()));
    if (dup)
      setRawString(dup);
    else
      setNull();
  }

  template <typename T>
  static void setRawString(VariantData* var, SerializedValue<T> value,
                           ResourceManager* resources) {
    if (!var)
      return;
    var->setRawString(value, resources);
  }

  template <typename TAdaptedString>
  void setString(TAdaptedString value, ResourceManager* resources) {
    setNull(resources);

    if (value.isNull())
      return;

    if (value.isLinked()) {
      setLinkedString(value.data());
      return;
    }

    auto dup = resources->saveString(value);
    if (dup)
      setOwnedString(dup);
  }

  template <typename TAdaptedString>
  static void setString(VariantData* var, TAdaptedString value,
                        ResourceManager* resources) {
    if (!var)
      return;
    var->setString(value, resources);
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

  size_t size(const ResourceManager* resources) const {
    return isCollection() ? content_.asCollection.size(resources) : 0;
  }

  static size_t size(const VariantData* var, const ResourceManager* resources) {
    return var != 0 ? var->size(resources) : 0;
  }

  ArrayData& toArray() {
    setType(VALUE_IS_ARRAY);
    new (&content_.asArray) ArrayData();
    return content_.asArray;
  }

  ArrayData& toArray(ResourceManager* resources) {
    release(resources);
    return toArray();
  }

  static ArrayData* toArray(VariantData* var, ResourceManager* resources) {
    if (!var)
      return 0;
    return &var->toArray(resources);
  }

  ObjectData& toObject() {
    setType(VALUE_IS_OBJECT);
    new (&content_.asObject) ObjectData();
    return content_.asObject;
  }

  ObjectData& toObject(ResourceManager* resources) {
    release(resources);
    return toObject();
  }

  static ObjectData* toObject(VariantData* var, ResourceManager* resources) {
    if (!var)
      return 0;
    return &var->toObject(resources);
  }

  uint8_t type() const {
    return flags_ & VALUE_MASK;
  }

 private:
  void release(ResourceManager* resources) {
    if (flags_ & OWNED_VALUE_BIT)
      resources->dereferenceString(content_.asOwnedString->data);

    auto collection = asCollection();
    if (collection)
      collection->clear(resources);
  }

  void setType(uint8_t t) {
    flags_ &= OWNED_KEY_BIT;
    flags_ |= t;
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
