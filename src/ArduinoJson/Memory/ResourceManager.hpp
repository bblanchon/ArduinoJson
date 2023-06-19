// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/Alignment.hpp>
#include <ArduinoJson/Memory/Allocator.hpp>
#include <ArduinoJson/Memory/StringNode.hpp>
#include <ArduinoJson/Memory/VariantPool.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/utility.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class VariantSlot;
class VariantPool;

class ResourceManager {
 public:
  ResourceManager(size_t capa,
                  Allocator* allocator = DefaultAllocator::instance())
      : allocator_(allocator), overflowed_(false) {
    variantPool_.create(addPadding(capa), allocator);
  }

  ~ResourceManager() {
    deallocAllStrings();
    variantPool_.destroy(allocator_);
  }

  ResourceManager(const ResourceManager&) = delete;
  ResourceManager& operator=(const ResourceManager& src) = delete;

  ResourceManager& operator=(ResourceManager&& src) {
    deallocAllStrings();
    variantPool_.destroy(allocator_);
    allocator_ = src.allocator_;
    variantPool_ = detail::move(src.variantPool_);
    overflowed_ = src.overflowed_;
    strings_ = src.strings_;
    src.strings_ = nullptr;
    return *this;
  }

  Allocator* allocator() const {
    return allocator_;
  }

  void reallocPool(size_t requiredSize) {
    size_t capa = addPadding(requiredSize);
    if (capa == capacity())
      return;
    variantPool_.destroy(allocator_);
    variantPool_.create(requiredSize, allocator_);
  }

  // Gets the capacity of the memoryPool in bytes
  size_t capacity() const {
    return variantPool_.capacity();
  }

  size_t size() const {
    size_t total = variantPool_.usage();
    for (auto node = strings_; node; node = node->next)
      total += sizeofString(node->length);
    return total;
  }

  bool overflowed() const {
    return overflowed_;
  }

  VariantSlot* allocVariant() {
    auto p = variantPool_.allocVariant();
    if (!p)
      overflowed_ = true;
    return p;
  }

  template <typename TAdaptedString>
  StringNode* saveString(TAdaptedString str) {
    if (str.isNull())
      return 0;

    auto node = findString(str);
    if (node) {
      node->references++;
      return node;
    }

    size_t n = str.size();

    node = allocString(n);
    if (!node)
      return nullptr;

    stringGetChars(str, node->data, n);
    node->data[n] = 0;  // force NUL terminator
    addStringToList(node);
    return node;
  }

  void addStringToList(StringNode* node) {
    ARDUINOJSON_ASSERT(node != nullptr);
    node->next = strings_;
    strings_ = node;
  }

  template <typename TAdaptedString>
  StringNode* findString(const TAdaptedString& str) const {
    for (auto node = strings_; node; node = node->next) {
      if (stringEquals(str, adaptString(node->data, node->length)))
        return node;
    }
    return nullptr;
  }

  StringNode* allocString(size_t length) {
    auto node = StringNode::create(length, allocator_);
    if (!node)
      overflowed_ = true;
    return node;
  }

  StringNode* reallocString(StringNode* node, size_t length) {
    node = StringNode::resize(node, length, allocator_);
    if (!node)
      overflowed_ = true;
    return node;
  }

  void deallocString(StringNode* node) {
    StringNode::destroy(node, allocator_);
  }

  void dereferenceString(const char* s) {
    StringNode* prev = nullptr;
    for (auto node = strings_; node; node = node->next) {
      if (node->data == s) {
        if (--node->references == 0) {
          if (prev)
            prev->next = node->next;
          else
            strings_ = node->next;
          StringNode::destroy(node, allocator_);
        }
        return;
      }
      prev = node;
    }
  }

  void clear() {
    variantPool_.clear();
    overflowed_ = false;
    deallocAllStrings();
  }

  ptrdiff_t shrinkToFit() {
    return variantPool_.shrinkToFit(allocator_);
  }

 private:
  void deallocAllStrings() {
    while (strings_) {
      auto node = strings_;
      strings_ = node->next;
      deallocString(node);
    }
  }

  Allocator* allocator_;
  bool overflowed_;
  StringNode* strings_ = nullptr;
  VariantPool variantPool_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
