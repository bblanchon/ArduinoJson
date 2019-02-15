// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "../Collection/CollectionData.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename Visitor>
void objectAccept(const CollectionData *obj, Visitor &visitor) {
  if (obj)
    visitor.visitObject(*obj);
  else
    visitor.visitNull();
}

template <typename TAdaptedString>
inline bool objectContainsKey(const CollectionData *obj, TAdaptedString key) {
  return obj && obj->containsKey(key);
}

inline bool objectEquals(const CollectionData *lhs, const CollectionData *rhs) {
  if (lhs == rhs) return true;
  if (!lhs || !rhs) return false;
  return lhs->equalsObject(*rhs);
}

template <typename TAdaptedString>
inline VariantData *objectGet(const CollectionData *obj, TAdaptedString key) {
  if (!obj) return 0;
  return obj->get(key);
}

template <typename TAdaptedString>
void objectRemove(CollectionData *obj, TAdaptedString key) {
  if (!obj) return;
  obj->remove(key);
}

template <typename TAdaptedString>
inline VariantData *objectGetOrCreate(CollectionData *obj, TAdaptedString key,
                                      MemoryPool *pool) {
  if (!obj) return 0;

  // ignore null key
  if (key.isNull()) return 0;

  // search a matching key
  VariantData *var = obj->get(key);
  if (var) return var;

  return obj->add(key, pool);
}
}  // namespace ARDUINOJSON_NAMESPACE
