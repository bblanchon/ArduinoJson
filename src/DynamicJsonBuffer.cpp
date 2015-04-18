// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "../include/ArduinoJson/DynamicJsonBuffer.hpp"

namespace ArduinoJson {
namespace Internals {
struct DynamicJsonBufferBlockWithoutData {
  DynamicJsonBufferBlock* next;
  size_t capacity;
  size_t size;
};


struct DynamicJsonBufferBlock : DynamicJsonBufferBlockWithoutData {
  uint8_t data[1];
};
}
}

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

DynamicJsonBuffer::DynamicJsonBuffer() {
  _head = createBlock(FIRST_BLOCK_CAPACITY);
}

DynamicJsonBuffer::~DynamicJsonBuffer() {
  Block* currentBlock = _head;

  while (currentBlock != NULL) {
    Block* nextBlock = currentBlock->next;
    free(currentBlock);
    currentBlock = nextBlock;
  }
}

size_t DynamicJsonBuffer::size() const {
  size_t total = 0;

  for (const Block* b = _head; b != NULL; b = b->next) {
    total += b->size;
  }

  return total;
}

void* DynamicJsonBuffer::alloc(size_t bytes) {
  if (!canAllocInHead(bytes)) addNewBlock();
  return allocInHead(bytes);
}

bool DynamicJsonBuffer::canAllocInHead(size_t bytes) const {
  return _head->size + bytes <= _head->capacity;
}

void* DynamicJsonBuffer::allocInHead(size_t bytes) {
  void* p = _head->data + _head->size;
  _head->size += bytes;
  return p;
}

void DynamicJsonBuffer::addNewBlock() {
  Block* block = createBlock(_head->capacity * 2);
  block->next = _head;
  _head = block;
}

DynamicJsonBuffer::Block* DynamicJsonBuffer::createBlock(size_t capacity) {
  size_t blkSize = sizeof(DynamicJsonBufferBlockWithoutData) + capacity;
  Block* block = static_cast<Block*>(malloc(blkSize));
  block->capacity = capacity;
  block->size = 0;
  block->next = NULL;
  return block;
}
