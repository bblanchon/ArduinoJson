// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <stdint.h>  // int8_t, int16_t

#include <ArduinoJson/Polyfills/gsl/not_null.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>
#include <ArduinoJson/Variant/VariantContent.hpp>

namespace ARDUINOJSON_NAMESPACE {

typedef conditional<sizeof(void*) <= 2, int8_t, int16_t>::type VariantSlotDiff;

class VariantSlot {
  // CAUTION: same layout as VariantData
  // we cannot use composition because it adds padding
  // (+20% on ESP8266 for example)
  VariantContent _content;
  uint8_t _flags;
  VariantSlotDiff _next;
  const char* _key;

 public:
  // Must be a POD!
  // - no constructor
  // - no destructor
  // - no virtual
  // - no inheritance

  VariantData* data() {
    return reinterpret_cast<VariantData*>(&_content);
  }

  const VariantData* data() const {
    return reinterpret_cast<const VariantData*>(&_content);
  }

  VariantSlot* next() {
    return _next ? this + _next : 0;
  }

  const VariantSlot* next() const {
    return const_cast<VariantSlot*>(this)->next();
  }

  VariantSlot* next(size_t distance) {
    VariantSlot* slot = this;
    while (distance--) {
      if (!slot->_next)
        return 0;
      slot += slot->_next;
    }
    return slot;
  }

  const VariantSlot* next(size_t distance) const {
    return const_cast<VariantSlot*>(this)->next(distance);
  }

  void setNext(VariantSlot* slot) {
    _next = VariantSlotDiff(slot ? slot - this : 0);
  }

  void setNextNotNull(VariantSlot* slot) {
    ARDUINOJSON_ASSERT(slot != 0);
    _next = VariantSlotDiff(slot - this);
  }

  void setKey(const char* k, storage_policies::store_by_copy) {
    ARDUINOJSON_ASSERT(k != NULL);
    _flags |= KEY_IS_OWNED;
    _key = k;
  }

  void setKey(const char* k, storage_policies::store_by_address) {
    ARDUINOJSON_ASSERT(k != NULL);
    _flags &= VALUE_MASK;
    _key = k;
  }

  const char* key() const {
    return _key;
  }

  bool ownsKey() const {
    return (_flags & KEY_IS_OWNED) != 0;
  }

  void clear() {
    _next = 0;
    _flags = 0;
    _key = 0;
  }

  void movePointers(ptrdiff_t stringDistance, ptrdiff_t variantDistance) {
    if (_flags & KEY_IS_OWNED)
      _key += stringDistance;
    if (_flags & VALUE_IS_OWNED)
      _content.asString += stringDistance;
    if (_flags & COLLECTION_MASK)
      _content.asCollection.movePointers(stringDistance, variantDistance);
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
