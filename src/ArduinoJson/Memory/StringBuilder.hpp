// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/ResourceManager.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class StringBuilder {
 public:
  static const size_t initialCapacity = 31;

  StringBuilder(ResourceManager* resources) : resources_(resources) {}

  ~StringBuilder() {
    if (node_)
      resources_->destroyString(node_);
  }

  void startString() {
    size_ = 0;
    if (!node_)
      node_ = resources_->createString(initialCapacity);
  }

  StringNode* save() {
    ARDUINOJSON_ASSERT(node_ != nullptr);
    node_->data[size_] = 0;
    StringNode* node = resources_->getString(adaptString(node_->data, size_));
    if (!node) {
      node = resources_->resizeString(node_, size_);
      ARDUINOJSON_ASSERT(node != nullptr);  // realloc to smaller can't fail
      resources_->saveString(node);
      node_ = nullptr;  // next time we need a new string
    } else {
      node->references++;
    }
    return node;
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
    if (node_ && size_ == node_->length)
      node_ = resources_->resizeString(node_, size_ * 2U + 1);
    if (node_)
      node_->data[size_++] = c;
  }

  bool isValid() const {
    return node_ != nullptr;
  }

  size_t size() const {
    return size_;
  }

  JsonString str() const {
    ARDUINOJSON_ASSERT(node_ != nullptr);
    node_->data[size_] = 0;
    return JsonString(node_->data, size_, JsonString::Copied);
  }

 private:
  ResourceManager* resources_;
  StringNode* node_ = nullptr;
  size_t size_ = 0;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
