// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Polyfills/integer.hpp>
#include <ArduinoJson/Polyfills/limits.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantContent.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

struct StringNode;

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
  // Placement new
  static void* operator new(size_t, void* p) noexcept {
    return p;
  }

  static void operator delete(void*, void*) noexcept {}

  VariantSlot() : flags_(0), next_(0), key_(0) {}

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

  void setKey(const char* k) {
    ARDUINOJSON_ASSERT(k);
    flags_ &= VALUE_MASK;
    key_ = k;
  }

  void setKey(StringNode* k) {
    ARDUINOJSON_ASSERT(k);
    flags_ |= OWNED_KEY_BIT;
    key_ = k->data;
  }

  const char* key() const {
    return key_;
  }

  bool ownsKey() const {
    return (flags_ & OWNED_KEY_BIT) != 0;
  }
};

inline VariantData* slotData(VariantSlot* slot) {
  return reinterpret_cast<VariantData*>(slot);
}

// Returns the size (in bytes) of an array with n elements.
constexpr size_t sizeofArray(size_t n) {
  return n * sizeof(VariantSlot);
}

// Returns the size (in bytes) of an object with n members.
constexpr size_t sizeofObject(size_t n) {
  return n * sizeof(VariantSlot);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
