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
  uint8_t type_;
  SlotId next_;

 public:
  // Placement new
  static void* operator new(size_t, void* p) noexcept {
    return p;
  }

  static void operator delete(void*, void*) noexcept {}

  VariantSlot() : type_(0), next_(NULL_SLOT) {
    (void)type_;  // HACK: suppress Clang warning "private field is not used"
  }

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
  return 2 * n * sizeof(VariantSlot);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
