// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/Alignment.hpp>
#include <ArduinoJson/Memory/Allocator.hpp>
#include <ArduinoJson/Memory/StringPool.hpp>
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
    stringPool_.clear(allocator_);
    variantPool_.destroy(allocator_);
  }

  ResourceManager(const ResourceManager&) = delete;
  ResourceManager& operator=(const ResourceManager& src) = delete;

  ResourceManager& operator=(ResourceManager&& src) {
    stringPool_.clear(allocator_);
    variantPool_.destroy(allocator_);
    allocator_ = src.allocator_;
    variantPool_ = detail::move(src.variantPool_);
    overflowed_ = src.overflowed_;
    stringPool_ = detail::move(src.stringPool_);
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
    return variantPool_.usage() + stringPool_.size();
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

    auto node = stringPool_.add(str, allocator_);
    if (!node)
      overflowed_ = true;

    return node;
  }

  void addStringToList(StringNode* node) {
    stringPool_.add(node);
  }

  template <typename TAdaptedString>
  StringNode* findString(const TAdaptedString& str) const {
    return stringPool_.get(str);
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
    stringPool_.dereference(s, allocator_);
  }

  void clear() {
    variantPool_.clear();
    overflowed_ = false;
    stringPool_.clear(allocator_);
  }

  ptrdiff_t shrinkToFit() {
    return variantPool_.shrinkToFit(allocator_);
  }

 private:
  Allocator* allocator_;
  bool overflowed_;
  StringPool stringPool_;
  VariantPool variantPool_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
