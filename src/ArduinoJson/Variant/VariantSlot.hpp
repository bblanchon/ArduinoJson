// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Polyfills/limits.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantContent.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

struct StringNode;

class VariantSlot {
  // CAUTION: same layout as VariantData
  // we cannot use composition because it adds padding
  // (+20% on ESP8266 for example)
  VariantContent content_;
  uint8_t flags_;
  SlotId next_;
  const char* key_;

 public:
  // Placement new
  static void* operator new(size_t, void* p) noexcept {
    return p;
  }

  static void operator delete(void*, void*) noexcept {}

  VariantSlot() : flags_(0), next_(NULL_SLOT), key_(0) {}

  VariantData* data() {
    return reinterpret_cast<VariantData*>(&content_);
  }

  const VariantData* data() const {
    return reinterpret_cast<const VariantData*>(&content_);
  }

  SlotId next() const {
    return next_;
  }

  void setNext(SlotId slot) {
    next_ = slot;
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
