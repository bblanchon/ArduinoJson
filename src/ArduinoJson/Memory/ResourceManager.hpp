// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/Alignment.hpp>
#include <ArduinoJson/Memory/Allocator.hpp>
#include <ArduinoJson/Memory/StringNode.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/mpl/max.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>

#include <string.h>  // memmove

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class ResourceManager {
 public:
  ResourceManager(size_t capa,
                  Allocator* allocator = DefaultAllocator::instance())
      : allocator_(allocator), overflowed_(false) {
    allocPool(addPadding(capa));
  }

  ~ResourceManager() {
    deallocAllStrings();
    deallocPool();
  }

  ResourceManager(const ResourceManager&) = delete;
  ResourceManager& operator=(const ResourceManager& src) = delete;

  ResourceManager& operator=(ResourceManager&& src) {
    deallocAllStrings();
    deallocPool();
    allocator_ = src.allocator_;
    pool_ = src.pool_;
    poolCapacity_ = src.poolCapacity_;
    poolUsage_ = src.poolUsage_;
    overflowed_ = src.overflowed_;
    src.pool_ = nullptr;
    src.poolCapacity_ = src.poolUsage_ = 0;
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
    allocator_->deallocate(pool_);
    allocPool(requiredSize);
  }

  // Gets the capacity of the memoryPool in bytes
  size_t capacity() const {
    return poolCapacity_;
  }

  size_t size() const {
    size_t total = poolUsage_;
    for (auto node = strings_; node; node = node->next)
      total += sizeofString(node->length);
    return total;
  }

  bool overflowed() const {
    return overflowed_;
  }

  void* allocFromPool(size_t bytes) {
    if (!canAlloc(bytes)) {
      overflowed_ = true;
      return 0;
    }
    auto p = pool_ + poolUsage_;
    poolUsage_ += bytes;
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
    auto node = reinterpret_cast<StringNode*>(
        allocator_->allocate(sizeofString(length)));
    if (node) {
      node->length = uint16_t(length);
      node->references = 1;
    } else {
      overflowed_ = true;
    }
    return node;
  }

  StringNode* reallocString(StringNode* node, size_t length) {
    ARDUINOJSON_ASSERT(node != nullptr);
    auto newNode = reinterpret_cast<StringNode*>(
        allocator_->reallocate(node, sizeofString(length)));
    if (newNode) {
      newNode->length = uint16_t(length);
    } else {
      overflowed_ = true;
      allocator_->deallocate(node);
    }
    return newNode;
  }

  void deallocString(StringNode* node) {
    allocator_->deallocate(node);
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
          allocator_->deallocate(node);
        }
        return;
      }
      prev = node;
    }
  }

  void clear() {
    poolUsage_ = 0;
    overflowed_ = false;
    deallocAllStrings();
  }

  bool canAlloc(size_t bytes) const {
    return poolUsage_ + bytes <= poolCapacity_;
  }

  // Workaround for missing placement new
  void* operator new(size_t, void* p) {
    return p;
  }

  ptrdiff_t shrinkToFit() {
    auto originalPoolAddress = pool_;
    pool_ = reinterpret_cast<char*>(allocator_->reallocate(pool_, poolUsage_));
    poolCapacity_ = poolUsage_;
    return pool_ - originalPoolAddress;
  }

 private:
  void deallocAllStrings() {
    while (strings_) {
      auto node = strings_;
      strings_ = node->next;
      deallocString(node);
    }
  }

  void allocPool(size_t capa) {
    pool_ = capa ? reinterpret_cast<char*>(allocator_->allocate(capa)) : 0;
    poolUsage_ = 0;
    poolCapacity_ = pool_ ? capa : 0;
    ARDUINOJSON_ASSERT(isAligned(pool_));
  }

  void deallocPool() {
    if (pool_)
      allocator_->deallocate(pool_);
  }

  Allocator* allocator_;
  char* pool_;
  size_t poolUsage_, poolCapacity_;
  bool overflowed_;
  StringNode* strings_ = nullptr;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
