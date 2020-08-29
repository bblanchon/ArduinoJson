// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/attributes.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TVisitor>
inline typename TVisitor::result_type variantAccept(const VariantData *var,
                                                    TVisitor &visitor) {
  if (var != 0)
    return var->accept(visitor);
  else
    return visitor.visitNull();
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
  if (!dst)
    return false;
  if (!src) {
    dst->setNull();
    return true;
  }
  return dst->copyFrom(*src, pool);
}

inline int variantCompare(const VariantData *a, const VariantData *b);

inline bool variantIsArray(const VariantData *var) {
  return var && var->isArray();
}

inline bool variantIsBoolean(const VariantData *var) {
  return var && var->isBoolean();
}

template <typename T>
inline bool variantIsInteger(const VariantData *var) {
  return var && var->isInteger<T>();
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
  if (!var)
    return false;
  var->setBoolean(value);
  return true;
}

inline bool variantSetFloat(VariantData *var, Float value) {
  if (!var)
    return false;
  var->setFloat(value);
  return true;
}

inline bool variantSetLinkedRaw(VariantData *var,
                                SerializedValue<const char *> value) {
  if (!var)
    return false;
  var->setLinkedRaw(value);
  return true;
}

template <typename T>
inline bool variantSetOwnedRaw(VariantData *var, SerializedValue<T> value,
                               MemoryPool *pool) {
  return var != 0 && var->setOwnedRaw(value, pool);
}

inline void variantSetNull(VariantData *var) {
  if (!var)
    return;
  var->setNull();
}

template <typename TAdaptedString>
inline bool variantSetString(VariantData *var, TAdaptedString value,
                             MemoryPool *pool) {
  if (!var)
    return false;
  return var->setString(value, pool);
}

template <typename T>
inline bool variantSetInteger(VariantData *var, T value) {
  ARDUINOJSON_ASSERT_INTEGER_TYPE_IS_SUPPORTED(T);
  if (!var)
    return false;
  var->setInteger(value);
  return true;
}

inline size_t variantSize(const VariantData *var) {
  return var != 0 ? var->size() : 0;
}

inline CollectionData *variantToArray(VariantData *var) {
  if (!var)
    return 0;
  return &var->toArray();
}

inline CollectionData *variantToObject(VariantData *var) {
  if (!var)
    return 0;
  return &var->toObject();
}

inline NO_INLINE VariantData *variantAddElement(VariantData *var,
                                                MemoryPool *pool) {
  return var != 0 ? var->addElement(pool) : 0;
}

inline NO_INLINE VariantData *variantGetOrAddElement(VariantData *var,
                                                     size_t index,
                                                     MemoryPool *pool) {
  return var != 0 ? var->getOrAddElement(index, pool) : 0;
}

template <typename TChar>
NO_INLINE VariantData *variantGetOrAddMember(VariantData *var, TChar *key,
                                             MemoryPool *pool) {
  return var != 0 ? var->getOrAddMember(adaptString(key), pool) : 0;
}

template <typename TString>
NO_INLINE VariantData *variantGetOrAddMember(VariantData *var,
                                             const TString &key,
                                             MemoryPool *pool) {
  return var != 0 ? var->getOrAddMember(adaptString(key), pool) : 0;
}

}  // namespace ARDUINOJSON_NAMESPACE
