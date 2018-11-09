// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Strings/StringInMemoryPool.hpp"
#include "Alignment.hpp"
#include "MemoryPool.hpp"
#include "StaticMemoryPool.hpp"

#include <stdlib.h>  // malloc, free

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#elif defined(__GNUC__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#endif
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

namespace ARDUINOJSON_NAMESPACE {
class DefaultAllocator {
 public:
  void* allocate(size_t size) {
    return malloc(size);
  }
  void deallocate(void* pointer) {
    free(pointer);
  }
};

template <typename TAllocator>
class DynamicMemoryPoolBase : public MemoryPool {
  class Block : public StaticMemoryPoolBase {
   public:
    Block(char* buf, size_t sz, Block* nxt)
        : StaticMemoryPoolBase(buf, sz), next(nxt) {}
    Block* next;
  };

 public:
  enum { EmptyBlockSize = sizeof(Block) };

  DynamicMemoryPoolBase(size_t initialSize = ARDUINOJSON_DEFAULT_POOL_SIZE)
      : _head(NULL), _nextBlockCapacity(initialSize) {}

  ~DynamicMemoryPoolBase() {
    clear();
  }

  void reserve(size_t capacity) {
    _nextBlockCapacity = capacity;
  }

  virtual size_t size() const {
    size_t sum = 0;
    for (Block* b = _head; b; b = b->next) {
      sum += b->size();
    }
    return sum;
  }

  virtual VariantSlot* allocVariant() {
    for (Block* b = _head; b; b = b->next) {
      VariantSlot* s = b->allocVariant();
      if (s) return s;
    }

    if (!addNewBlock(sizeof(VariantSlot))) return 0;

    return _head->allocVariant();
  }

  virtual void freeVariant(VariantSlot* slot) {
    for (Block* b = _head; b; b = b->next) {
      if (b->owns(slot)) {
        b->freeVariant(slot);
        break;
      }
    }
  }

  virtual void freeString(StringSlot* slot) {
    for (Block* b = _head; b; b = b->next) {
      if (b->owns(slot)) {
        b->freeString(slot);
        break;
      }
    }
  }

  virtual StringSlot* allocFrozenString(size_t n) {
    for (Block* b = _head; b; b = b->next) {
      StringSlot* s = b->allocFrozenString(n);
      if (s) return s;
    }

    if (!addNewBlock(sizeof(StringSlot) + n)) return 0;

    return _head->allocFrozenString(n);
  }

  virtual StringSlot* allocExpandableString() {
    for (Block* b = _head; b; b = b->next) {
      StringSlot* s = b->allocExpandableString();
      if (s) return s;
    }

    if (!addNewBlock(sizeof(StringSlot))) return 0;

    return _head->allocExpandableString();
  }

  virtual StringSlot* expandString(StringSlot* oldSlot) {
    if (!addNewBlock(sizeof(StringSlot) + oldSlot->size)) return 0;

    StringSlot* newSlot = _head->allocExpandableString();

    ARDUINOJSON_ASSERT(newSlot->size > oldSlot->size);
    memcpy(newSlot->value, oldSlot->value, oldSlot->size);
    freeString(oldSlot);

    return newSlot;
  }

  virtual void freezeString(StringSlot* slot, size_t newSize) {
    for (Block* b = _head; b; b = b->next) {
      if (b->owns(slot)) {
        b->freezeString(slot, newSize);
      }
    }
  }

  // Resets the memoryPool.
  // USE WITH CAUTION: this invalidates all previously allocated data
  void clear() {
    Block* currentBlock = _head;
    while (currentBlock != NULL) {
      _nextBlockCapacity = currentBlock->capacity();
      Block* nextBlock = currentBlock->next;
      _allocator.deallocate(currentBlock);
      currentBlock = nextBlock;
    }
    _head = 0;
  }

  size_t blockCount() const {
    size_t sum = 0;
    for (Block* b = _head; b; b = b->next) sum++;
    return sum;
  }

 private:
  bool addNewBlock(size_t minCapacity) {
    size_t capacity = _nextBlockCapacity;
    if (minCapacity > capacity) capacity = minCapacity;
    capacity = addPadding(capacity);
    size_t bytes = sizeof(Block) + capacity;
    char* p = reinterpret_cast<char*>(_allocator.allocate(bytes));
    if (!p) return false;
    Block* block = new (p) Block(p + sizeof(Block), capacity, _head);
    _nextBlockCapacity = capacity * 2;
    _head = block;
    return true;
  }

  TAllocator _allocator;
  Block* _head;
  size_t _nextBlockCapacity;
};

// Implements a MemoryPool with dynamic memory allocation.
// You are strongly encouraged to consider using StaticMemoryPool which is much
// more suitable for embedded systems.
typedef DynamicMemoryPoolBase<DefaultAllocator> DynamicMemoryPool;
}  // namespace ARDUINOJSON_NAMESPACE

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif
#endif
