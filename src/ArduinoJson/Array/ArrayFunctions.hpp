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
}  // namespace ARDUINOJSON_NAMESPACE
