// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>

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

  // Array only

  VariantData *addElement(MemoryPool *pool);

  VariantData *getElement(size_t index) const;

  VariantData *getOrAddElement(size_t index, MemoryPool *pool);

  void removeElement(size_t index);

  bool equalsArray(const CollectionData &other) const;

  // Object only

  template <typename TAdaptedString>
  VariantData *addMember(TAdaptedString key, MemoryPool *pool);

  template <typename TAdaptedString>
  VariantData *getMember(TAdaptedString key) const;

  template <typename TAdaptedString>
  VariantData *getOrAddMember(TAdaptedString key, MemoryPool *pool);

  template <typename TAdaptedString>
  void removeMember(TAdaptedString key) {
    removeSlot(getSlot(key));
  }

  template <typename TAdaptedString>
  bool containsKey(const TAdaptedString &key) const;

  bool equalsObject(const CollectionData &other) const;

  // Generic

  void clear();
  size_t memoryUsage() const;
  size_t nesting() const;
  size_t size() const;

  VariantSlot *addSlot(MemoryPool *);
  void removeSlot(VariantSlot *slot);

  bool copyFrom(const CollectionData &src, MemoryPool *pool);

  VariantSlot *head() const {
    return _head;
  }

  void movePointers(ptrdiff_t stringDistance, ptrdiff_t variantDistance);

 private:
  VariantSlot *getSlot(size_t index) const;

  template <typename TAdaptedString>
  VariantSlot *getSlot(TAdaptedString key) const;

  VariantSlot *getPreviousSlot(VariantSlot *) const;
};
}  // namespace ARDUINOJSON_NAMESPACE
