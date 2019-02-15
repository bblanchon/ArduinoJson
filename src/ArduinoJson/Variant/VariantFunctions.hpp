// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "VariantData.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename Visitor>
inline void variantAccept(const VariantData *var, Visitor &visitor) {
  if (var != 0)
    var->accept(visitor);
  else
    visitor.visitNull();
}

inline const CollectionData *variantAsArray(const VariantData *var) {
  return var != 0 ? var->asArray() : 0;
}

inline const CollectionData *variantAsObject(const VariantData *var) {
  return var != 0 ? var->asObject() : 0;
}

inline CollectionData *variantAsObject(VariantData *var) {
  return var != 0 ? var->asObject() : 0;
}

inline bool variantCopyFrom(VariantData *dst, const VariantData *src,
                            MemoryPool *pool) {
  if (!dst) return false;
  if (!src) {
    dst->setNull();
    return true;
  }
  return dst->copyFrom(*src, pool);
}

inline bool variantEquals(const VariantData *a, const VariantData *b) {
  if (a == b) return true;
  if (!a || !b) return false;
  return a->equals(*b);
}

inline bool variantIsArray(const VariantData *var) {
  return var && var->isArray();
}

inline bool variantIsBoolean(const VariantData *var) {
  return var && var->isBoolean();
}

inline bool variantIsInteger(const VariantData *var) {
  return var && var->isInteger();
}

inline bool variantIsFloat(const VariantData *var) {
  return var && var->isFloat();
}

inline bool variantIsString(const VariantData *var) {
  return var && var->isString();
}

inline bool variantIsObject(const VariantData *var) {
  return var && var->isObject();
}

inline bool variantIsNull(const VariantData *var) {
  return var == 0 || var->isNull();
}

inline bool variantSetBoolean(VariantData *var, bool value) {
  if (!var) return false;
  var->setBoolean(value);
  return true;
}

inline bool variantSetFloat(VariantData *var, Float value) {
  if (!var) return false;
  var->setFloat(value);
  return true;
}

inline bool variantSetLinkedRaw(VariantData *var,
                                SerializedValue<const char *> value) {
  if (!var) return false;
  var->setLinkedRaw(value);
  return true;
}

template <typename T>
inline bool variantSetOwnedRaw(VariantData *var, SerializedValue<T> value,
                               MemoryPool *pool) {
  return var != 0 && var->setOwnedRaw(value, pool);
}

template <typename T>
inline bool variantSetSignedInteger(VariantData *var, T value) {
  if (!var) return false;
  var->setSignedInteger(value);
  return true;
}

inline bool variantSetLinkedString(VariantData *var, const char *value) {
  if (!var) return false;
  var->setLinkedString(value);
  return true;
}

inline void variantSetNull(VariantData *var) {
  if (!var) return;
  var->setNull();
}

inline bool variantSetOwnedString(VariantData *var, char *value) {
  if (!var) return false;
  var->setOwnedString(value);
  return true;
}

template <typename T>
inline bool variantSetOwnedString(VariantData *var, T value, MemoryPool *pool) {
  return var != 0 && var->setOwnedString(value, pool);
}

inline bool variantSetUnsignedInteger(VariantData *var, UInt value) {
  if (!var) return false;
  var->setUnsignedInteger(value);
  return true;
}

inline size_t variantSize(const VariantData *var) {
  return var != 0 ? var->size() : 0;
}

inline CollectionData *variantToArray(VariantData *var) {
  if (!var) return 0;
  return &var->toArray();
}

inline CollectionData *variantToObject(VariantData *var) {
  if (!var) return 0;
  return &var->toObject();
}

inline NO_INLINE VariantData *variantAdd(VariantData *var, MemoryPool *pool) {
  return var != 0 ? var->addElement(pool) : 0;
}

template <typename TChar>
NO_INLINE VariantData *variantGetOrCreate(VariantData *var, TChar *key,
                                          MemoryPool *pool) {
  return var != 0 ? var->getOrAddMember(adaptString(key), pool) : 0;
}

template <typename TString>
NO_INLINE VariantData *variantGetOrCreate(VariantData *var, const TString &key,
                                          MemoryPool *pool) {
  return var != 0 ? var->getOrAddMember(adaptString(key), pool) : 0;
}

}  // namespace ARDUINOJSON_NAMESPACE
