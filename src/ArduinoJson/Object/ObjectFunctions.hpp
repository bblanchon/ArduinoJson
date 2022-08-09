// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TAdaptedString>
inline VariantData* objectGetMember(const CollectionData* obj,
                                    TAdaptedString key) {
  if (!obj)
    return 0;
  return obj->getMember(key);
}

template <typename TAdaptedString>
void objectRemove(CollectionData* obj, TAdaptedString key) {
  if (!obj)
    return;
  obj->removeMember(key);
}
}  // namespace ARDUINOJSON_NAMESPACE
