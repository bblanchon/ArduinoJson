// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>

namespace ARDUINOJSON_NAMESPACE {

inline VariantData *arrayAdd(CollectionData *arr, MemoryPool *pool) {
  return arr ? arr->addElement(pool) : 0;
}

template <typename TVisitor>
inline typename TVisitor::result_type arrayAccept(const CollectionData *arr,
                                                  TVisitor &visitor) {
  if (arr)
    return visitor.visitArray(*arr);
  else
    return visitor.visitNull();
}

inline bool arrayEquals(const CollectionData *lhs, const CollectionData *rhs) {
  if (lhs == rhs)
    return true;
  if (!lhs || !rhs)
    return false;
  return lhs->equalsArray(*rhs);
}
}  // namespace ARDUINOJSON_NAMESPACE
