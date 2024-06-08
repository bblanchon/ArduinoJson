// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/Allocator.hpp>
#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/integer.hpp>
#include <ArduinoJson/Polyfills/limits.hpp>

#include <stddef.h>  // offsetof

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

struct StringNode {
  // Use the same type as SlotId to store the reference count
  // (there can never be more references than slots)
  using references_type = uint_t<ARDUINOJSON_SLOT_ID_SIZE * 8>;

  using length_type = uint_t<ARDUINOJSON_STRING_LENGTH_SIZE * 8>;

  struct StringNode* next;
  references_type references;
  length_type length;
  char data[1];

  static constexpr size_t maxLength = numeric_limits<length_type>::highest();

  static constexpr size_t sizeForLength(size_t n) {
    return n + 1 + offsetof(StringNode, data);
  }

  static StringNode* create(size_t length, Allocator* allocator) {
    if (length > maxLength)
      return nullptr;
    auto size = sizeForLength(length);
    if (size < length)  // integer overflow
      return nullptr;   // (not testable on 64-bit)
    auto node = reinterpret_cast<StringNode*>(allocator->allocate(size));
    if (node) {
      node->length = length_type(length);
      node->references = 1;
    }
    return node;
  }

  static StringNode* resize(StringNode* node, size_t length,
                            Allocator* allocator) {
    ARDUINOJSON_ASSERT(node != nullptr);
    StringNode* newNode;
    if (length <= maxLength)
      newNode = reinterpret_cast<StringNode*>(
          allocator->reallocate(node, sizeForLength(length)));
    else
      newNode = nullptr;
    if (newNode)
      newNode->length = length_type(length);
    else
      allocator->deallocate(node);
    return newNode;
  }

  static void destroy(StringNode* node, Allocator* allocator) {
    allocator->deallocate(node);
  }
};

// Returns the size (in bytes) of an string with n characters.
constexpr size_t sizeofString(size_t n) {
  return StringNode::sizeForLength(n);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
