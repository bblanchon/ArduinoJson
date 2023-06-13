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

class MemoryPool {
 public:
  MemoryPool(size_t capa, Allocator* allocator = DefaultAllocator::instance())
      : allocator_(allocator), overflowed_(false) {
    allocPool(addPadding(capa));
  }

  ~MemoryPool() {
    deallocAllStrings();
    deallocPool();
  }

  MemoryPool(const MemoryPool&) = delete;
  MemoryPool& operator=(const MemoryPool& src) = delete;

  MemoryPool& operator=(MemoryPool&& src) {
    deallocAllStrings();
    deallocPool();
    allocator_ = src.allocator_;
    begin_ = src.begin_;
    end_ = src.end_;
    right_ = src.right_;
    overflowed_ = src.overflowed_;
    src.begin_ = src.end_ = src.right_ = nullptr;
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
    allocator_->deallocate(begin_);
    allocPool(requiredSize);
  }

  // Gets the capacity of the memoryPool in bytes
  size_t capacity() const {
    return size_t(end_ - begin_);
  }

  size_t size() const {
    size_t total = size_t(end_ - right_);
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
    right_ -= bytes;
    return right_;
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
    right_ = end_;
    overflowed_ = false;
    deallocAllStrings();
  }

  bool canAlloc(size_t bytes) const {
    return begin_ + bytes <= right_;
  }

  // Workaround for missing placement new
  void* operator new(size_t, void* p) {
    return p;
  }

  ptrdiff_t shrinkToFit() {
    ptrdiff_t bytes_reclaimed = squash();
    if (bytes_reclaimed == 0)
      return 0;

    void* old_ptr = begin_;
    void* new_ptr = allocator_->reallocate(old_ptr, capacity());

    ptrdiff_t ptr_offset =
        static_cast<char*>(new_ptr) - static_cast<char*>(old_ptr);

    movePointers(ptr_offset);
    return ptr_offset - bytes_reclaimed;
  }

 private:
  ptrdiff_t squash() {
    char* new_right = addPadding(begin_);
    if (new_right >= right_)
      return 0;

    size_t right_size = static_cast<size_t>(end_ - right_);
    memmove(new_right, right_, right_size);

    ptrdiff_t bytes_reclaimed = right_ - new_right;
    right_ = new_right;
    end_ = new_right + right_size;
    return bytes_reclaimed;
  }

  // Move all pointers together
  // This funcion is called after a realloc.
  void movePointers(ptrdiff_t offset) {
    begin_ += offset;
    right_ += offset;
    end_ += offset;
  }

  void deallocAllStrings() {
    while (strings_) {
      auto node = strings_;
      strings_ = node->next;
      deallocString(node);
    }
  }

  void allocPool(size_t capa) {
    auto buf = capa ? reinterpret_cast<char*>(allocator_->allocate(capa)) : 0;
    begin_ = buf;
    end_ = right_ = buf ? buf + capa : 0;
    ARDUINOJSON_ASSERT(isAligned(begin_));
    ARDUINOJSON_ASSERT(isAligned(right_));
    ARDUINOJSON_ASSERT(isAligned(end_));
  }

  void deallocPool() {
    if (begin_)
      allocator_->deallocate(begin_);
  }

  Allocator* allocator_;
  char *begin_, *right_, *end_;
  bool overflowed_;
  StringNode* strings_ = nullptr;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
