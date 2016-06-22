// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../JsonBuffer.hpp"

#include <stdlib.h>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#elif defined(__GNUC__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#endif
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

namespace ArduinoJson {
namespace Internals {
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
class BlockJsonBuffer : public JsonBuffer {
  struct Block;
  struct EmptyBlock {
    Block* next;
    size_t capacity;
    size_t size;
  };
  struct Block : EmptyBlock {
    uint8_t data[1];
  };

 public:
  BlockJsonBuffer(size_t initialSize = 256)
      : _head(NULL), _nextBlockSize(initialSize) {}

  ~BlockJsonBuffer() {
    Block* currentBlock = _head;

    while (currentBlock != NULL) {
      Block* nextBlock = currentBlock->next;
      _allocator.deallocate(currentBlock);
      currentBlock = nextBlock;
    }
  }

  size_t size() const {
    size_t total = 0;
    for (const Block* b = _head; b; b = b->next) total += b->size;
    return total;
  }

  virtual void* alloc(size_t bytes) {
    return canAllocInHead(bytes) ? allocInHead(bytes) : allocInNewBlock(bytes);
  }

 private:
  bool canAllocInHead(size_t bytes) const {
    return _head != NULL && _head->size + bytes <= _head->capacity;
  }

  void* allocInHead(size_t bytes) {
    void* p = _head->data + _head->size;
    _head->size += round_size_up(bytes);
    return p;
  }

  void* allocInNewBlock(size_t bytes) {
    size_t capacity = _nextBlockSize;
    if (bytes > capacity) capacity = bytes;
    if (!addNewBlock(capacity)) return NULL;
    _nextBlockSize *= 2;
    return allocInHead(bytes);
  }

  bool addNewBlock(size_t capacity) {
    size_t bytes = sizeof(EmptyBlock) + capacity;
    Block* block = static_cast<Block*>(_allocator.allocate(bytes));
    if (block == NULL) return false;
    block->capacity = capacity;
    block->size = 0;
    block->next = _head;
    _head = block;
    return true;
  }

  TAllocator _allocator;
  Block* _head;
  size_t _nextBlockSize;
};
}
}

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif
#endif
