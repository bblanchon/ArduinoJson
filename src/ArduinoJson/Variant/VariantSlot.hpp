// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/integer.hpp>
#include <ArduinoJson/Polyfills/limits.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantContent.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

typedef int_t<ARDUINOJSON_SLOT_OFFSET_SIZE * 8>::type VariantSlotDiff;

class VariantSlot {
  // CAUTION: same layout as VariantData
  // we cannot use composition because it adds padding
  // (+20% on ESP8266 for example)
  VariantContent content_;
  uint8_t flags_;
  VariantSlotDiff next_;
  const char* key_;

 public:
  // Must be a POD!
  // - no constructor
  // - no destructor
  // - no virtual
  // - no inheritance

  VariantData* data() {
    return reinterpret_cast<VariantData*>(&content_);
  }

  const VariantData* data() const {
    return reinterpret_cast<const VariantData*>(&content_);
  }

  VariantSlot* next() {
    return next_ ? this + next_ : 0;
  }

  const VariantSlot* next() const {
    return const_cast<VariantSlot*>(this)->next();
  }

  VariantSlot* next(size_t distance) {
    VariantSlot* slot = this;
    while (distance--) {
      if (!slot->next_)
        return 0;
      slot += slot->next_;
    }
    return slot;
  }

  const VariantSlot* next(size_t distance) const {
    return const_cast<VariantSlot*>(this)->next(distance);
  }

  void setNext(VariantSlot* slot) {
    ARDUINOJSON_ASSERT(!slot || slot - this >=
                                    numeric_limits<VariantSlotDiff>::lowest());
    ARDUINOJSON_ASSERT(!slot || slot - this <=
                                    numeric_limits<VariantSlotDiff>::highest());
    next_ = VariantSlotDiff(slot ? slot - this : 0);
  }

  void setNextNotNull(VariantSlot* slot) {
    ARDUINOJSON_ASSERT(slot != 0);
    ARDUINOJSON_ASSERT(slot - this >=
                       numeric_limits<VariantSlotDiff>::lowest());
    ARDUINOJSON_ASSERT(slot - this <=
                       numeric_limits<VariantSlotDiff>::highest());
    next_ = VariantSlotDiff(slot - this);
  }

  void setKey(JsonString k) {
    ARDUINOJSON_ASSERT(k);
    if (k.isLinked())
      flags_ &= VALUE_MASK;
    else
      flags_ |= OWNED_KEY_BIT;
    key_ = k.c_str();
  }

  const char* key() const {
    return key_;
  }

  bool ownsKey() const {
    return (flags_ & OWNED_KEY_BIT) != 0;
  }

  void clear() {
    next_ = 0;
    flags_ = 0;
    key_ = 0;
  }

  void movePointers(ptrdiff_t stringDistance, ptrdiff_t variantDistance) {
    if (flags_ & OWNED_KEY_BIT)
      key_ += stringDistance;
    if (flags_ & OWNED_VALUE_BIT)
      content_.asString.data += stringDistance;
    if (flags_ & COLLECTION_MASK)
      content_.asCollection.movePointers(stringDistance, variantDistance);
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
