// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "../Misc/SerializedValue.hpp"
#include "../Numbers/convertNumber.hpp"
#include "../Polyfills/gsl/not_null.hpp"
#include "VariantContent.hpp"

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

  template <typename Visitor>
  void accept(Visitor &visitor) const {
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
        return setOwnedString(RamStringAdapter(src._content.asString), pool);
      case VALUE_IS_OWNED_RAW:
        return setOwnedRaw(
            serialized(src._content.asRaw.data, src._content.asRaw.size), pool);
      default:
        setType(src.type());
        _content = src._content;
        return true;
    }
  }

  bool equals(const VariantData &other) const {
    if (type() != other.type()) return false;

    switch (type()) {
      case VALUE_IS_LINKED_STRING:
      case VALUE_IS_OWNED_STRING:
        return !strcmp(_content.asString, other._content.asString);

      case VALUE_IS_LINKED_RAW:
      case VALUE_IS_OWNED_RAW:
        return _content.asRaw.size == other._content.asRaw.size &&
               !memcmp(_content.asRaw.data, other._content.asRaw.data,
                       _content.asRaw.size);

      case VALUE_IS_BOOLEAN:
      case VALUE_IS_POSITIVE_INTEGER:
      case VALUE_IS_NEGATIVE_INTEGER:
        return _content.asInteger == other._content.asInteger;

      case VALUE_IS_ARRAY:
        return _content.asCollection.equalsArray(other._content.asCollection);

      case VALUE_IS_OBJECT:
        return _content.asCollection.equalsObject(other._content.asCollection);

      case VALUE_IS_FLOAT:
        return _content.asFloat == other._content.asFloat;

      case VALUE_IS_NULL:
      default:
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

  void remove(size_t index) {
    if (isArray()) _content.asCollection.remove(index);
  }

  template <typename TAdaptedString>
  void remove(TAdaptedString key) {
    if (isObject()) _content.asCollection.remove(key);
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
    char *dup = adaptString(value.data(), value.size()).save(pool);
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

  void setPositiveInteger(UInt value) {
    setType(VALUE_IS_POSITIVE_INTEGER);
    _content.asInteger = value;
  }

  void setNegativeInteger(UInt value) {
    setType(VALUE_IS_NEGATIVE_INTEGER);
    _content.asInteger = value;
  }

  void setLinkedString(const char *value) {
    if (value) {
      setType(VALUE_IS_LINKED_STRING);
      _content.asString = value;
    } else {
      setType(VALUE_IS_NULL);
    }
  }

  void setNull() {
    setType(VALUE_IS_NULL);
  }

  void setOwnedString(not_null<const char *> s) {
    setType(VALUE_IS_OWNED_STRING);
    _content.asString = s.get();
  }

  bool setOwnedString(const char *s) {
    if (s) {
      setOwnedString(make_not_null(s));
      return true;
    } else {
      setType(VALUE_IS_NULL);
      return false;
    }
  }

  template <typename T>
  bool setOwnedString(T value, MemoryPool *pool) {
    return setOwnedString(value.save(pool));
  }

  void setUnsignedInteger(UInt value) {
    setType(VALUE_IS_POSITIVE_INTEGER);
    _content.asInteger = static_cast<UInt>(value);
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
    if (isNull()) toArray();
    if (!isArray()) return 0;
    return _content.asCollection.add(pool);
  }

  VariantData *getElement(size_t index) const {
    return isArray() ? _content.asCollection.get(index) : 0;
  }

  template <typename TAdaptedString>
  VariantData *getMember(TAdaptedString key) const {
    return isObject() ? _content.asCollection.get(key) : 0;
  }

  template <typename TAdaptedString>
  VariantData *getOrAddMember(TAdaptedString key, MemoryPool *pool) {
    if (isNull()) toObject();
    if (!isObject()) return 0;
    VariantData *var = _content.asCollection.get(key);
    if (var) return var;
    return _content.asCollection.add(key, pool);
  }

 private:
  uint8_t type() const {
    return _flags & VALUE_MASK;
  }

  void setType(uint8_t t) {
    _flags &= KEY_IS_OWNED;
    _flags |= t;
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
