// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TVisitor>
typename TVisitor::result_type objectAccept(const CollectionData *obj,
                                            TVisitor &visitor) {
  if (obj)
    return visitor.visitObject(*obj);
  else
    return visitor.visitNull();
}

inline bool objectEquals(const CollectionData *lhs, const CollectionData *rhs) {
  if (lhs == rhs)
    return true;
  if (!lhs || !rhs)
    return false;
  return lhs->equalsObject(*rhs);
}

template <typename TAdaptedString>
inline VariantData *objectGetMember(const CollectionData *obj,
                                    TAdaptedString key) {
  if (!obj)
    return 0;
  return obj->getMember(key);
}

template <typename TAdaptedString>
void objectRemove(CollectionData *obj, TAdaptedString key) {
  if (!obj)
    return;
  obj->removeMember(key);
}

template <typename TAdaptedString>
inline VariantData *objectGetOrAddMember(CollectionData *obj,
                                         TAdaptedString key, MemoryPool *pool) {
  if (!obj)
    return 0;

  return obj->getOrAddMember(key, pool);
}
}  // namespace ARDUINOJSON_NAMESPACE
