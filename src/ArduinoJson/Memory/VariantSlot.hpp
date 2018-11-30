// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Data/JsonVariantData.hpp"
#include "../Polyfills/type_traits.hpp"

namespace ARDUINOJSON_NAMESPACE {

typedef conditional<sizeof(void*) <= 2, int8_t, int16_t>::type VariantSlotDiff;

struct VariantSlot {
  JsonVariantData value;
  VariantSlotDiff next;
  VariantSlotDiff prev;
  const char* key;

  // Must be a POD! so no constructor, nor destructor, nor virtual

  VariantSlot* getNext() {
    return next ? this + next : 0;
  }

  const VariantSlot* getNext() const {
    return const_cast<VariantSlot*>(this)->getNext();
  }

  VariantSlot* getNext(size_t distance) {
    VariantSlot* slot = this;
    while (distance--) {
      if (!slot->next) return 0;
      slot += slot->next;
    }
    return slot;
  }

  const VariantSlot* getNext(size_t distance) const {
    return const_cast<VariantSlot*>(this)->getNext(distance);
  }

  VariantSlot* getPrev() {
    return prev ? this + prev : 0;
  }

  void setNext(VariantSlot* slot) {
    this->next = VariantSlotDiff(slot ? slot - this : 0);
  }

  void setPrev(VariantSlot* slot) {
    this->prev = VariantSlotDiff(slot ? slot - this : 0);
  }

  void attachTo(VariantSlot* tail) {
    VariantSlotDiff offset = VariantSlotDiff(tail - this);
    this->prev = offset;
    tail->next = VariantSlotDiff(-offset);
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
