// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../JsonBuffer.hpp"

#include <stdlib.h>

namespace ArduinoJson {
namespace Internals {
class DefaultAllocator {
 public:
  void* allocate(size_t size) { return malloc(size); }
  void deallocate(void* pointer) { free(pointer); }
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
  BlockJsonBuffer() : _head(NULL) {}

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

 protected:
  virtual void* alloc(size_t bytes) {
    return canAllocInHead(bytes) ? allocInHead(bytes) : allocInNewBlock(bytes);
  }

 private:
  static const size_t FIRST_BLOCK_CAPACITY = 32;

  bool canAllocInHead(size_t bytes) const {
    return _head != NULL && _head->size + bytes <= _head->capacity;
  }

  void* allocInHead(size_t bytes) {
    void* p = _head->data + _head->size;
    _head->size += round_size_up(bytes);
    return p;
  }

  void* allocInNewBlock(size_t bytes) {
    size_t capacity = FIRST_BLOCK_CAPACITY;
    if (_head != NULL) capacity = _head->capacity * 2;
    if (bytes > capacity) capacity = bytes;
    if (!addNewBlock(capacity)) return NULL;
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

  Block* _head;
  TAllocator _allocator;
};
}
}
