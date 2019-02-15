// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "../Collection/CollectionData.hpp"

namespace ARDUINOJSON_NAMESPACE {

inline VariantData *arrayAdd(CollectionData *arr, MemoryPool *pool) {
  return arr ? arr->add(pool) : 0;
}

template <typename Visitor>
inline void arrayAccept(const CollectionData *arr, Visitor &visitor) {
  if (arr)
    visitor.visitArray(*arr);
  else
    visitor.visitNull();
}

inline bool arrayEquals(const CollectionData *lhs, const CollectionData *rhs) {
  if (lhs == rhs) return true;
  if (!lhs || !rhs) return false;

  return lhs->equalsArray(*rhs);
}
}  // namespace ARDUINOJSON_NAMESPACE
