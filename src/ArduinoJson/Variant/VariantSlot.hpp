// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Polyfills/type_traits.hpp"
#include "../Variant/VariantData.hpp"

namespace ARDUINOJSON_NAMESPACE {

typedef conditional<sizeof(void*) <= 2, int8_t, int16_t>::type VariantSlotDiff;

class VariantSlot {
  // CAUTION: same layout as VariantData
  // we cannot use composition because it adds padding
  // (+20% on ESP8266 for example)
  VariantContent _content;
  bool _keyIsOwned : 1;
  VariantType _type : 7;
  VariantSlotDiff _next;
  const char* _key;

 public:
  // Must be a POD!
  // - no constructor
  // - no destructor
  // - no virtual
  // - no inheritance

  VariantData* getData() {
    return reinterpret_cast<VariantData*>(&_content);
  }

  const VariantData* getData() const {
    return reinterpret_cast<const VariantData*>(&_content);
  }

  VariantSlot* getNext() {
    return _next ? this + _next : 0;
  }

  const VariantSlot* getNext() const {
    return const_cast<VariantSlot*>(this)->getNext();
  }

  VariantSlot* getNext(size_t distance) {
    VariantSlot* slot = this;
    while (distance--) {
      if (!slot->_next) return 0;
      slot += slot->_next;
    }
    return slot;
  }

  VariantSlot* getPrev(VariantSlot* head) {
    while (head) {
      VariantSlot* nxt = head->getNext();
      if (nxt == this) return head;
      head = nxt;
    }
    return head;
  }

  const VariantSlot* getNext(size_t distance) const {
    return const_cast<VariantSlot*>(this)->getNext(distance);
  }

  void setNext(VariantSlot* slot) {
    _next = VariantSlotDiff(slot ? slot - this : 0);
  }

  void attachTo(VariantSlot* tail) {
    tail->_next = VariantSlotDiff(this - tail);
  }

  void setKey(const char* k, bool owned) {
    _keyIsOwned = owned;
    _key = k;
  }

  const char* key() const {
    return _key;
  }

  bool ownsKey() const {
    return _keyIsOwned;
  }

  void init() {
    _next = 0;
    _type = JSON_NULL;
    _keyIsOwned = false;
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
