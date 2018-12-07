// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

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

  template <typename TKey>
  VariantData *add(TKey key, MemoryPool *pool);

  void clear();

  template <typename TKey>
  bool containsKey(const TKey &key) const;

  bool copyFrom(const CollectionData &src, MemoryPool *pool);

  bool equalsArray(const CollectionData &other) const;
  bool equalsObject(const CollectionData &other) const;

  VariantData *get(size_t index) const;

  template <typename TKey>
  VariantData *get(TKey key) const;

  VariantSlot *head() const {
    return _head;
  }

  void remove(size_t index);

  template <typename TKey>
  void remove(TKey key) {
    remove(getSlot(key));
  }

  void remove(VariantSlot *slot);

  size_t size() const;

 private:
  VariantSlot *getSlot(size_t index) const;

  template <typename TKey>
  VariantSlot *getSlot(TKey key) const;

  VariantSlot *getPreviousSlot(VariantSlot *) const;
};
}  // namespace ARDUINOJSON_NAMESPACE
