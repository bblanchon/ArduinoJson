// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stddef.h>  // for size_t

namespace ARDUINOJSON_NAMESPACE {

template <typename TSlot>
class SlotList {
 public:
  SlotList() : _head(0) {}

  TSlot *pop() {
    TSlot *slot = _head;
    if (slot) _head = slot->next;
    return slot;
  }

  void push(TSlot *slot) {
    slot->next = _head;
    _head = slot;
  }

  bool remove(const TSlot *slot) {
    if (_head == slot) {
      _head = slot->next;
      return true;
    }

    for (TSlot *s = _head; s; s = s->next) {
      if (s->next == slot) {
        s->next = slot->next;
        return true;
      }
    }

    return false;
  }

  bool remove(const void *slot) {
    return remove(reinterpret_cast<const TSlot *>(slot));
  }

  template <typename Functor>
  void forEach(const Functor &f) {
    for (TSlot *s = _head; s; s = s->next) {
      f(s);
    }
  }

  size_t size() const {
    size_t sum = 0;
    for (TSlot *s = _head; s; s = s->next) sum += sizeof(TSlot);
    return sum;
  }

  void clear() {
    _head = 0;
  }

 private:
  TSlot *_head;
};
}  // namespace ARDUINOJSON_NAMESPACE
