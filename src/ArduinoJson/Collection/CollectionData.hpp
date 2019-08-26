// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <stddef.h>  // size_t

namespace ARDUINOJSON_NAMESPACE {

class MemoryPool;
class VariantData;
class VariantSlot;

class CollectionData {
  VariantSlot *_head;
  VariantSlot *_tail;

 public:
  // Must be a POD!
  // - no constructor
  // - no destructor
  // - no virtual
  // - no inheritance
  VariantSlot *addSlot(MemoryPool *);

  VariantData *add(MemoryPool *pool);

  template <typename TAdaptedString>
  VariantData *add(TAdaptedString key, MemoryPool *pool);

  void clear();

  template <typename TAdaptedString>
  bool containsKey(const TAdaptedString &key) const;

  bool copyFrom(const CollectionData &src, MemoryPool *pool);

  bool equalsArray(const CollectionData &other) const;
  bool equalsObject(const CollectionData &other) const;

  VariantData *get(size_t index) const;

  template <typename TAdaptedString>
  VariantData *get(TAdaptedString key) const;

  VariantSlot *head() const {
    return _head;
  }

  void remove(size_t index);

  template <typename TAdaptedString>
  void remove(TAdaptedString key) {
    remove(getSlot(key));
  }

  void remove(VariantSlot *slot);

  size_t memoryUsage() const;
  size_t nesting() const;
  size_t size() const;

 private:
  VariantSlot *getSlot(size_t index) const;

  template <typename TAdaptedString>
  VariantSlot *getSlot(TAdaptedString key) const;

  VariantSlot *getPreviousSlot(VariantSlot *) const;
};
}  // namespace ARDUINOJSON_NAMESPACE
