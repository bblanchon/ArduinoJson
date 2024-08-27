// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Memory/StringNode.hpp>
#include <ArduinoJson/Misc/SerializedValue.hpp>
#include <ArduinoJson/Numbers/convertNumber.hpp>
#include <ArduinoJson/Strings/JsonString.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <ArduinoJson/Variant/VariantContent.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename T>
T parseNumber(const char* s);

class VariantData {
  VariantContent content_;  // must be first to allow cast from array to variant
  uint8_t type_;
  SlotId next_;

 public:
  // Placement new
  static void* operator new(size_t, void* p) noexcept {
    return p;
  }

  static void operator delete(void*, void*) noexcept {}

  VariantData() : type_(VALUE_IS_NULL), next_(NULL_SLOT) {}

  SlotId next() const {
    return next_;
  }

  void setNext(SlotId slot) {
    next_ = slot;
  }

  template <typename TVisitor>
  typename TVisitor::result_type accept(
      TVisitor& visit, const ResourceManager* resources) const {
    (void)resources;  // silence warning
    switch (type_) {
      case VALUE_IS_FLOAT:
        return visit.visit(static_cast<JsonFloat>(content_.asFloat));

#if ARDUINOJSON_USE_DOUBLE
      case VALUE_IS_DOUBLE:
        return visit.visit(getExtension(resources)->asDouble);
#endif

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

      case VALUE_IS_INT32:
        return visit.visit(static_cast<JsonInteger>(content_.asInt32));

      case VALUE_IS_UINT32:
        return visit.visit(static_cast<JsonUInt>(content_.asUint32));

#if ARDUINOJSON_USE_LONG_LONG
      case VALUE_IS_INT64:
        return visit.visit(getExtension(resources)->asInt64);

      case VALUE_IS_UINT64:
        return visit.visit(getExtension(resources)->asUint64);
#endif

      case VALUE_IS_BOOLEAN:
        return visit.visit(content_.asBoolean != 0);

      default:
        return visit.visit(nullptr);
    }
  }

  template <typename TVisitor>
  static typename TVisitor::result_type accept(const VariantData* var,
                                               const ResourceManager* resources,
                                               TVisitor& visit) {
    if (var != 0)
      return var->accept(visit, resources);
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

  template <typename T>
  bool addValue(T&& value, ResourceManager* resources) {
    auto array = isNull() ? &toArray() : asArray();
    return detail::ArrayData::addValue(array, detail::forward<T>(value),
                                       resources);
  }

  template <typename T>
  static bool addValue(VariantData* var, T&& value,
                       ResourceManager* resources) {
    if (!var)
      return false;
    return var->addValue(value, resources);
  }

  bool asBoolean(const ResourceManager* resources) const {
    (void)resources;  // silence warning
    switch (type_) {
      case VALUE_IS_BOOLEAN:
        return content_.asBoolean;
      case VALUE_IS_UINT32:
      case VALUE_IS_INT32:
        return content_.asUint32 != 0;
      case VALUE_IS_FLOAT:
        return content_.asFloat != 0;
#if ARDUINOJSON_USE_DOUBLE
      case VALUE_IS_DOUBLE:
        return getExtension(resources)->asDouble != 0;
#endif
      case VALUE_IS_NULL:
        return false;
#if ARDUINOJSON_USE_LONG_LONG
      case VALUE_IS_UINT64:
      case VALUE_IS_INT64:
        return getExtension(resources)->asUint64 != 0;
#endif
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
  T asFloat(const ResourceManager* resources) const {
    static_assert(is_floating_point<T>::value, "T must be a floating point");
    (void)resources;  // silence warning
    switch (type_) {
      case VALUE_IS_BOOLEAN:
        return static_cast<T>(content_.asBoolean);
      case VALUE_IS_UINT32:
        return static_cast<T>(content_.asUint32);
      case VALUE_IS_INT32:
        return static_cast<T>(content_.asInt32);
#if ARDUINOJSON_USE_LONG_LONG
      case VALUE_IS_UINT64:
        return static_cast<T>(getExtension(resources)->asUint64);
      case VALUE_IS_INT64:
        return static_cast<T>(getExtension(resources)->asInt64);
#endif
      case VALUE_IS_LINKED_STRING:
      case VALUE_IS_OWNED_STRING:
        return parseNumber<T>(content_.asOwnedString->data);
      case VALUE_IS_FLOAT:
        return static_cast<T>(content_.asFloat);
#if ARDUINOJSON_USE_DOUBLE
      case VALUE_IS_DOUBLE:
        return static_cast<T>(getExtension(resources)->asDouble);
#endif
      default:
        return 0;
    }
  }

  template <typename T>
  T asIntegral(const ResourceManager* resources) const {
    static_assert(is_integral<T>::value, "T must be an integral type");
    (void)resources;  // silence warning
    switch (type_) {
      case VALUE_IS_BOOLEAN:
        return content_.asBoolean;
      case VALUE_IS_UINT32:
        return convertNumber<T>(content_.asUint32);
      case VALUE_IS_INT32:
        return convertNumber<T>(content_.asInt32);
#if ARDUINOJSON_USE_LONG_LONG
      case VALUE_IS_UINT64:
        return convertNumber<T>(getExtension(resources)->asUint64);
      case VALUE_IS_INT64:
        return convertNumber<T>(getExtension(resources)->asInt64);
#endif
      case VALUE_IS_LINKED_STRING:
        return parseNumber<T>(content_.asLinkedString);
      case VALUE_IS_OWNED_STRING:
        return parseNumber<T>(content_.asOwnedString->data);
      case VALUE_IS_FLOAT:
        return convertNumber<T>(content_.asFloat);
#if ARDUINOJSON_USE_DOUBLE
      case VALUE_IS_DOUBLE:
        return convertNumber<T>(getExtension(resources)->asDouble);
#endif
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
    switch (type_) {
      case VALUE_IS_RAW_STRING:
        return JsonString(content_.asOwnedString->data,
                          content_.asOwnedString->length, JsonString::Copied);
      default:
        return JsonString();
    }
  }

  JsonString asString() const {
    switch (type_) {
      case VALUE_IS_LINKED_STRING:
        return JsonString(content_.asLinkedString, JsonString::Linked);
      case VALUE_IS_OWNED_STRING:
        return JsonString(content_.asOwnedString->data,
                          content_.asOwnedString->length, JsonString::Copied);
      default:
        return JsonString();
    }
  }

#if ARDUINOJSON_USE_EXTENSIONS
  const VariantExtension* getExtension(const ResourceManager* resources) const;
#endif

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
    return type_ == VALUE_IS_ARRAY;
  }

  bool isBoolean() const {
    return type_ == VALUE_IS_BOOLEAN;
  }

  bool isCollection() const {
    return (type_ & COLLECTION_MASK) != 0;
  }

  bool isFloat() const {
    return (type_ & NUMBER_BIT) != 0;
  }

  template <typename T>
  bool isInteger(const ResourceManager* resources) const {
    (void)resources;  // silence warning
    switch (type_) {
      case VALUE_IS_UINT32:
        return canConvertNumber<T>(content_.asUint32);

      case VALUE_IS_INT32:
        return canConvertNumber<T>(content_.asInt32);

#if ARDUINOJSON_USE_LONG_LONG
      case VALUE_IS_UINT64:
        return canConvertNumber<T>(getExtension(resources)->asUint64);

      case VALUE_IS_INT64:
        return canConvertNumber<T>(getExtension(resources)->asInt64);
#endif

      default:
        return false;
    }
  }

  bool isNull() const {
    return type_ == VALUE_IS_NULL;
  }

  static bool isNull(const VariantData* var) {
    if (!var)
      return true;
    return var->isNull();
  }

  bool isObject() const {
    return type_ == VALUE_IS_OBJECT;
  }

  bool isString() const {
    return type_ == VALUE_IS_LINKED_STRING || type_ == VALUE_IS_OWNED_STRING;
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

  void reset() {  // TODO: remove
    type_ = VALUE_IS_NULL;
  }

  void setBoolean(bool value) {
    ARDUINOJSON_ASSERT(type_ == VALUE_IS_NULL);  // must call clear() first
    type_ = VALUE_IS_BOOLEAN;
    content_.asBoolean = value;
  }

  template <typename T>
  enable_if_t<sizeof(T) == 4, bool> setFloat(T value, ResourceManager*) {
    ARDUINOJSON_ASSERT(type_ == VALUE_IS_NULL);  // must call clear() first
    type_ = VALUE_IS_FLOAT;
    content_.asFloat = value;
    return true;
  }

  template <typename T>
  enable_if_t<sizeof(T) == 8, bool> setFloat(T value, ResourceManager*);

  template <typename T>
  enable_if_t<is_signed<T>::value> setInteger(T value,
                                              ResourceManager* resources);

  template <typename T>
  enable_if_t<is_unsigned<T>::value> setInteger(T value,
                                                ResourceManager* resources);

  void setRawString(StringNode* s) {
    ARDUINOJSON_ASSERT(type_ == VALUE_IS_NULL);  // must call clear() first
    ARDUINOJSON_ASSERT(s);
    type_ = VALUE_IS_RAW_STRING;
    content_.asOwnedString = s;
  }

  template <typename T>
  void setRawString(SerializedValue<T> value, ResourceManager* resources);

  template <typename T>
  static void setRawString(VariantData* var, SerializedValue<T> value,
                           ResourceManager* resources) {
    if (!var)
      return;
    var->clear(resources);
    var->setRawString(value, resources);
  }

  template <typename TAdaptedString>
  bool setString(TAdaptedString value, ResourceManager* resources);

  bool setString(StringNode* s, ResourceManager*) {
    setOwnedString(s);
    return true;
  }

  template <typename TAdaptedString>
  static void setString(VariantData* var, TAdaptedString value,
                        ResourceManager* resources) {
    if (!var)
      return;
    var->clear(resources);
    var->setString(value, resources);
  }

  void setLinkedString(const char* s) {
    ARDUINOJSON_ASSERT(type_ == VALUE_IS_NULL);  // must call clear() first
    ARDUINOJSON_ASSERT(s);
    type_ = VALUE_IS_LINKED_STRING;
    content_.asLinkedString = s;
  }

  void setOwnedString(StringNode* s) {
    ARDUINOJSON_ASSERT(type_ == VALUE_IS_NULL);  // must call clear() first
    ARDUINOJSON_ASSERT(s);
    type_ = VALUE_IS_OWNED_STRING;
    content_.asOwnedString = s;
  }

  size_t size(const ResourceManager* resources) const {
    if (isObject())
      return content_.asObject.size(resources);

    if (isArray())
      return content_.asArray.size(resources);

    return 0;
  }

  static size_t size(const VariantData* var, const ResourceManager* resources) {
    return var != 0 ? var->size(resources) : 0;
  }

  ArrayData& toArray() {
    ARDUINOJSON_ASSERT(type_ == VALUE_IS_NULL);  // must call clear() first
    type_ = VALUE_IS_ARRAY;
    new (&content_.asArray) ArrayData();
    return content_.asArray;
  }

  static ArrayData* toArray(VariantData* var, ResourceManager* resources) {
    if (!var)
      return 0;
    var->clear(resources);
    return &var->toArray();
  }

  ObjectData& toObject() {
    ARDUINOJSON_ASSERT(type_ == VALUE_IS_NULL);  // must call clear() first
    type_ = VALUE_IS_OBJECT;
    new (&content_.asObject) ObjectData();
    return content_.asObject;
  }

  static ObjectData* toObject(VariantData* var, ResourceManager* resources) {
    if (!var)
      return 0;
    var->clear(resources);
    return &var->toObject();
  }

  uint8_t type() const {
    return type_;
  }

  // Release the resources used by this variant and set it to null.
  void clear(ResourceManager* resources);

  static void clear(VariantData* var, ResourceManager* resources) {
    if (!var)
      return;
    var->clear(resources);
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
