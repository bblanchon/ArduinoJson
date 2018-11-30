// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonVariantData.hpp"
#include "SlotFunctions.hpp"

namespace ARDUINOJSON_NAMESPACE {

inline JsonVariantData* arrayAdd(JsonArrayData* arr, MemoryPool* pool) {
  if (!arr) return 0;

  VariantSlot* slot = pool->allocVariant();
  if (!slot) return 0;

  slot->next = 0;
  slot->value.type = JSON_NULL;
  slot->value.keyIsOwned = false;

  if (arr->tail) {
    slot->attachTo(arr->tail);
    arr->tail = slot;
  } else {
    slot->prev = 0;
    arr->head = slot;
    arr->tail = slot;
  }

  return &slot->value;
}

inline VariantSlot* arrayGetSlot(const JsonArrayData* arr, size_t index) {
  if (!arr) return 0;
  return arr->head->getNext(index);
}

inline JsonVariantData* arrayGet(const JsonArrayData* arr, size_t index) {
  VariantSlot* slot = arrayGetSlot(arr, index);
  return slot ? &slot->value : 0;
}

inline void arrayRemove(JsonArrayData* arr, VariantSlot* slot) {
  if (!arr || !slot) return;

  if (slot->prev)
    slot->getPrev()->setNext(slot->getNext());
  else
    arr->head = slot->getNext();
  if (slot->next)
    slot->getNext()->setPrev(slot->getPrev());
  else
    arr->tail = slot->getPrev();
}

inline void arrayRemove(JsonArrayData* arr, size_t index) {
  arrayRemove(arr, arrayGetSlot(arr, index));
}

inline void arrayClear(JsonArrayData* arr) {
  if (!arr) return;
  arr->head = 0;
  arr->tail = 0;
}

bool variantCopy(JsonVariantData*, const JsonVariantData*, MemoryPool*);

inline bool arrayCopy(JsonArrayData* dst, const JsonArrayData* src,
                      MemoryPool* pool) {
  if (!dst || !src) return false;
  arrayClear(dst);
  for (VariantSlot* s = src->head; s; s = s->getNext()) {
    if (!variantCopy(arrayAdd(dst, pool), &s->value, pool)) return false;
  }
  return true;
}

bool variantEquals(const JsonVariantData*, const JsonVariantData*);

inline bool arrayEquals(const JsonArrayData* a1, const JsonArrayData* a2) {
  if (a1 == a2) return true;
  if (!a1 || !a2) return false;
  VariantSlot* s1 = a1->head;
  VariantSlot* s2 = a2->head;
  for (;;) {
    if (s1 == s2) return true;
    if (!s1 || !s2) return false;
    if (!variantEquals(&s1->value, &s2->value)) return false;
    s1 = s1->getNext();
    s2 = s2->getNext();
  }
}

inline size_t arraySize(const JsonArrayData* arr) {
  if (!arr) return 0;
  return slotSize(arr->head);
}
}  // namespace ARDUINOJSON_NAMESPACE
