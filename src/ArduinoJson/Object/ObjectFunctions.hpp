// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
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

template <typename TKey>
inline bool objectContainsKey(const CollectionData *obj, TKey key) {
  return obj && obj->containsKey(key);
}

inline bool objectEquals(const CollectionData *lhs, const CollectionData *rhs) {
  if (lhs == rhs) return true;
  if (!lhs || !rhs) return false;
  return lhs->equalsObject(*rhs);
}

template <typename TKey>
inline VariantData *objectGet(const CollectionData *obj, TKey key) {
  if (!obj) return 0;
  return obj->get(key);
}

template <typename TKey>
void objectRemove(CollectionData *obj, TKey key) {
  if (!obj) return;
  obj->remove(key);
}

template <typename TKey>
inline VariantData *objectSet(CollectionData *obj, TKey key, MemoryPool *pool) {
  if (!obj) return 0;

  // ignore null key
  if (key.isNull()) return 0;

  // search a matching key
  VariantData *var = obj->get(key);
  if (var) return var;

  return obj->add(key, pool);
}
}  // namespace ARDUINOJSON_NAMESPACE
