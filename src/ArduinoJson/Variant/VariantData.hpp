// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Misc/SerializedValue.hpp>
#include <ArduinoJson/Numbers/convertNumber.hpp>
#include <ArduinoJson/Strings/RamStringAdapter.hpp>
#include <ArduinoJson/Variant/VariantContent.hpp>

// VariantData can't have a constructor (to be a POD), so we have no way to fix
// this warning
#if defined(__GNUC__)
#if __GNUC__ >= 7
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#pragma GCC diagnostic ignored "-Wuninitialized"
#endif
#endif

namespace ARDUINOJSON_NAMESPACE {

class VariantData {
  VariantContent _content;  // must be first to allow cast from array to variant
  uint8_t _flags;

 public:
  // Must be a POD!
  // - no constructor
  // - no destructor
  // - no virtual
  // - no inheritance
  void init() {
    _flags = 0;
  }

  template <typename TVisitor>
  typename TVisitor::result_type accept(TVisitor &visitor) const {
    switch (type()) {
      case VALUE_IS_FLOAT:
        return visitor.visitFloat(_content.asFloat);

      case VALUE_IS_ARRAY:
        return visitor.visitArray(_content.asCollection);

      case VALUE_IS_OBJECT:
        return visitor.visitObject(_content.asCollection);

      case VALUE_IS_LINKED_STRING:
      case VALUE_IS_OWNED_STRING:
        return visitor.visitString(_content.asString);

      case VALUE_IS_OWNED_RAW:
      case VALUE_IS_LINKED_RAW:
        return visitor.visitRawJson(_content.asRaw.data, _content.asRaw.size);

      case VALUE_IS_NEGATIVE_INTEGER:
        return visitor.visitNegativeInteger(_content.asInteger);

      case VALUE_IS_POSITIVE_INTEGER:
        return visitor.visitPositiveInteger(_content.asInteger);

      case VALUE_IS_BOOLEAN:
        return visitor.visitBoolean(_content.asInteger != 0);

      default:
        return visitor.visitNull();
    }
  }

  template <typename T>
  T asIntegral() const;

  template <typename T>
  T asFloat() const;

  const char *asString() const;

  bool asBoolean() const;

  CollectionData *asArray() {
    return isArray() ? &_content.asCollection : 0;
  }

  const CollectionData *asArray() const {
    return const_cast<VariantData *>(this)->asArray();
  }

  CollectionData *asObject() {
    return isObject() ? &_content.asCollection : 0;
  }

  const CollectionData *asObject() const {
    return const_cast<VariantData *>(this)->asObject();
  }

  bool copyFrom(const VariantData &src, MemoryPool *pool) {
    switch (src.type()) {
      case VALUE_IS_ARRAY:
        return toArray().copyFrom(src._content.asCollection, pool);
      case VALUE_IS_OBJECT:
        return toObject().copyFrom(src._content.asCollection, pool);
      case VALUE_IS_OWNED_STRING:
        return setString(RamStringAdapter(src._content.asString), pool);
      case VALUE_IS_OWNED_RAW:
        return setOwnedRaw(
            serialized(src._content.asRaw.data, src._content.asRaw.size), pool);
      default:
        setType(src.type());
        _content = src._content;
        return true;
    }
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
      case VALUE_IS_POSITIVE_INTEGER:
        return canStorePositiveInteger<T>(_content.asInteger);

      case VALUE_IS_NEGATIVE_INTEGER:
        return canStoreNegativeInteger<T>(_content.asInteger);

      default:
        return false;
    }
  }

  bool isFloat() const {
    return type() == VALUE_IS_FLOAT || type() == VALUE_IS_POSITIVE_INTEGER ||
           type() == VALUE_IS_NEGATIVE_INTEGER;
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

  void remove(size_t index) {
    if (isArray())
      _content.asCollection.removeElement(index);
  }

  template <typename TAdaptedString>
  void remove(TAdaptedString key) {
    if (isObject())
      _content.asCollection.removeMember(key);
  }

  void setBoolean(bool value) {
    setType(VALUE_IS_BOOLEAN);
    _content.asInteger = static_cast<UInt>(value);
  }

  void setFloat(Float value) {
    setType(VALUE_IS_FLOAT);
    _content.asFloat = value;
  }

  void setLinkedRaw(SerializedValue<const char *> value) {
    if (value.data()) {
      setType(VALUE_IS_LINKED_RAW);
      _content.asRaw.data = value.data();
      _content.asRaw.size = value.size();
    } else {
      setType(VALUE_IS_NULL);
    }
  }

  template <typename T>
  bool setOwnedRaw(SerializedValue<T> value, MemoryPool *pool) {
    const char *dup = pool->saveString(adaptString(value.data(), value.size()));
    if (dup) {
      setType(VALUE_IS_OWNED_RAW);
      _content.asRaw.data = dup;
      _content.asRaw.size = value.size();
      return true;
    } else {
      setType(VALUE_IS_NULL);
      return false;
    }
  }

  template <typename T>
  typename enable_if<is_unsigned<T>::value>::type setInteger(T value) {
    setUnsignedInteger(value);
  }

  template <typename T>
  typename enable_if<is_signed<T>::value>::type setInteger(T value) {
    setSignedInteger(value);
  }

  template <typename T>
  void setSignedInteger(T value) {
    if (value >= 0) {
      setPositiveInteger(static_cast<UInt>(value));
    } else {
      setNegativeInteger(~static_cast<UInt>(value) + 1);
    }
  }

  void setUnsignedInteger(UInt value) {
    setType(VALUE_IS_POSITIVE_INTEGER);
    _content.asInteger = static_cast<UInt>(value);
  }

  void setPositiveInteger(UInt value) {
    setType(VALUE_IS_POSITIVE_INTEGER);
    _content.asInteger = value;
  }

  void setNegativeInteger(UInt value) {
    setType(VALUE_IS_NEGATIVE_INTEGER);
    _content.asInteger = value;
  }

  void setNull() {
    setType(VALUE_IS_NULL);
  }

  void setStringPointer(const char *s, storage_policies::store_by_copy) {
    setType(VALUE_IS_OWNED_STRING);
    _content.asString = s;
  }

  void setStringPointer(const char *s, storage_policies::store_by_address) {
    setType(VALUE_IS_LINKED_STRING);
    _content.asString = s;
  }

  template <typename TAdaptedString>
  bool setString(TAdaptedString value, MemoryPool *pool) {
    return setString(value, pool, typename TAdaptedString::storage_policy());
  }

  template <typename TAdaptedString>
  inline bool setString(TAdaptedString value, MemoryPool *pool,
                        storage_policies::decide_at_runtime) {
    if (value.isStatic())
      return setString(value, pool, storage_policies::store_by_address());
    else
      return setString(value, pool, storage_policies::store_by_copy());
  }

  template <typename TAdaptedString>
  inline bool setString(TAdaptedString value, MemoryPool *,
                        storage_policies::store_by_address) {
    if (value.isNull())
      setNull();
    else
      setStringPointer(value.data(), storage_policies::store_by_address());
    return true;
  }

  template <typename TAdaptedString>
  inline bool setString(TAdaptedString value, MemoryPool *pool,
                        storage_policies::store_by_copy) {
    if (value.isNull()) {
      setNull();
      return true;
    }
    const char *copy = pool->saveString(value);
    if (!copy) {
      setNull();
      return false;
    }
    setStringPointer(copy, storage_policies::store_by_copy());
    return true;
  }

  CollectionData &toArray() {
    setType(VALUE_IS_ARRAY);
    _content.asCollection.clear();
    return _content.asCollection;
  }

  CollectionData &toObject() {
    setType(VALUE_IS_OBJECT);
    _content.asCollection.clear();
    return _content.asCollection;
  }

  size_t memoryUsage() const {
    switch (type()) {
      case VALUE_IS_OWNED_STRING:
        return strlen(_content.asString) + 1;
      case VALUE_IS_OWNED_RAW:
        return _content.asRaw.size;
      case VALUE_IS_OBJECT:
      case VALUE_IS_ARRAY:
        return _content.asCollection.memoryUsage();
      default:
        return 0;
    }
  }

  size_t nesting() const {
    return isCollection() ? _content.asCollection.nesting() : 0;
  }

  size_t size() const {
    return isCollection() ? _content.asCollection.size() : 0;
  }

  VariantData *addElement(MemoryPool *pool) {
    if (isNull())
      toArray();
    if (!isArray())
      return 0;
    return _content.asCollection.addElement(pool);
  }

  VariantData *getElement(size_t index) const {
    return isArray() ? _content.asCollection.getElement(index) : 0;
  }

  VariantData *getOrAddElement(size_t index, MemoryPool *pool) {
    if (isNull())
      toArray();
    if (!isArray())
      return 0;
    return _content.asCollection.getOrAddElement(index, pool);
  }

  template <typename TAdaptedString>
  VariantData *getMember(TAdaptedString key) const {
    return isObject() ? _content.asCollection.getMember(key) : 0;
  }

  template <typename TAdaptedString>
  VariantData *getOrAddMember(TAdaptedString key, MemoryPool *pool) {
    if (isNull())
      toObject();
    if (!isObject())
      return 0;
    return _content.asCollection.getOrAddMember(key, pool);
  }

  void movePointers(ptrdiff_t stringDistance, ptrdiff_t variantDistance) {
    if (_flags & VALUE_IS_OWNED)
      _content.asString += stringDistance;
    if (_flags & COLLECTION_MASK)
      _content.asCollection.movePointers(stringDistance, variantDistance);
  }

  uint8_t type() const {
    return _flags & VALUE_MASK;
  }

 private:
  void setType(uint8_t t) {
    _flags &= KEY_IS_OWNED;
    _flags |= t;
  }
};

}  // namespace ARDUINOJSON_NAMESPACE

#if defined(__GNUC__)
#if __GNUC__ >= 8
#pragma GCC diagnostic pop
#endif
#endif
