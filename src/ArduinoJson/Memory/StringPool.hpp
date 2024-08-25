// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/Allocator.hpp>
#include <ArduinoJson/Memory/StringNode.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/utility.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class StringPool {
 public:
  StringPool() = default;
  StringPool(const StringPool&) = delete;
  void operator=(StringPool&& src) = delete;

  ~StringPool() {
    ARDUINOJSON_ASSERT(strings_ == nullptr);
  }

  friend void swap(StringPool& a, StringPool& b) {
    swap_(a.strings_, b.strings_);
  }

  void clear(Allocator* allocator) {
    while (strings_) {
      auto node = strings_;
      strings_ = node->next;
      StringNode::destroy(node, allocator);
    }
  }

  size_t size() const {
    size_t total = 0;
    for (auto node = strings_; node; node = node->next)
      total += sizeofString(node->length);
    return total;
  }

  template <typename TAdaptedString>
  StringNode* add(TAdaptedString str, Allocator* allocator) {
    ARDUINOJSON_ASSERT(str.isNull() == false);

    auto node = get(str);
    if (node) {
      node->references++;
      return node;
    }

    size_t n = str.size();

    node = StringNode::create(n, allocator);
    if (!node)
      return nullptr;

    stringGetChars(str, node->data, n);
    node->data[n] = 0;  // force NUL terminator
    add(node);
    return node;
  }

  void add(StringNode* node) {
    ARDUINOJSON_ASSERT(node != nullptr);
    node->next = strings_;
    strings_ = node;
  }

  template <typename TAdaptedString>
  StringNode* get(const TAdaptedString& str) const {
    for (auto node = strings_; node; node = node->next) {
      if (stringEquals(str, adaptString(node->data, node->length)))
        return node;
    }
    return nullptr;
  }

  void dereference(const char* s, Allocator* allocator) {
    StringNode* prev = nullptr;
    for (auto node = strings_; node; node = node->next) {
      if (node->data == s) {
        if (--node->references == 0) {
          if (prev)
            prev->next = node->next;
          else
            strings_ = node->next;
          StringNode::destroy(node, allocator);
        }
        return;
      }
      prev = node;
    }
  }

 private:
  StringNode* strings_ = nullptr;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
