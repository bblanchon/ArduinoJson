// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/MemoryPool.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class StringCopier {
 public:
  static const size_t initialCapacity = 31;

  StringCopier(MemoryPool* pool) : _pool(pool) {}

  ~StringCopier() {
    if (_node)
      _pool->deallocString(_node);
  }

  void startString() {
    _size = 0;
    if (!_node)
      _node = _pool->allocString(initialCapacity);
  }

  JsonString save() {
    ARDUINOJSON_ASSERT(_node != nullptr);
    _node->data[_size] = 0;
    StringNode* node = _pool->findString(adaptString(_node->data, _size));
    if (!node) {
      node = _pool->reallocString(_node, _size);
      _pool->addStringToList(node);
      _node = nullptr;  // next time we need a new string
    }
    return JsonString(node->data, node->length, JsonString::Copied);
  }

  void append(const char* s) {
    while (*s)
      append(*s++);
  }

  void append(const char* s, size_t n) {
    while (n-- > 0)  // TODO: memcpy
      append(*s++);
  }

  void append(char c) {
    if (_node && _size == _node->length)
      _node = _pool->reallocString(_node, _size * 2U + 1);
    if (_node)
      _node->data[_size++] = c;
  }

  bool isValid() const {
    return _node != nullptr;
  }

  size_t size() const {
    return _size;
  }

  JsonString str() const {
    ARDUINOJSON_ASSERT(_node != nullptr);
    _node->data[_size] = 0;
    return JsonString(_node->data, _size, JsonString::Copied);
  }

 private:
  MemoryPool* _pool;
  StringNode* _node = nullptr;
  size_t _size = 0;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
