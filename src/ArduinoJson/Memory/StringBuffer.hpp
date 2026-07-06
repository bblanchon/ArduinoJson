// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Strings/JsonString.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class StringBuffer {
 public:
  StringBuffer(ResourceManager* resources) : resources_(resources) {}

  ~StringBuffer() {
    if (node_)
      resources_->destroyString(node_);
  }

  char* reserve(size_t capacity) {
    if (node_ && capacity > node_->length) {
      // existing buffer is too small, we need to reallocate
      resources_->destroyString(node_);
      node_ = nullptr;
    }
    if (!node_)
      node_ = resources_->createString(capacity);
    if (!node_)
      return nullptr;
    size_ = capacity;
    node_->data[capacity] = 0;  // null-terminate the string
    return node_->data;
  }

  JsonString str() const {
    ARDUINOJSON_ASSERT(node_ != nullptr);
    return JsonString(node_->data, node_->length);
  }

  void save(VariantData* data) {
    ARDUINOJSON_ASSERT(node_ != nullptr);
    const char* s = node_->data;
    if (isTinyString(s, size_))
      data->setTinyString(adaptString(s, size_));
    else
      data->setLongString(commitStringNode());
  }

  void saveRaw(VariantData* data) {
    data->setRawString(commitStringNode());
  }

 private:
  StringNode* commitStringNode() {
    ARDUINOJSON_ASSERT(node_ != nullptr);
    node_->data[size_] = 0;
    auto node = resources_->getString(adaptString(node_->data, size_));
    if (node) {
      node->references++;
      return node;
    }

    if (node_->length != size_) {
      node = resources_->resizeString(node_, size_);
      ARDUINOJSON_ASSERT(node != nullptr);  // realloc to smaller can't fail
    } else {
      node = node_;
    }
    node_ = nullptr;
    resources_->saveString(node);
    return node;
  }

  ResourceManager* resources_;
  StringNode* node_ = nullptr;
  size_t size_ = 0;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
