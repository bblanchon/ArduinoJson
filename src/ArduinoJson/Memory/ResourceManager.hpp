// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/Allocator.hpp>
#include <ArduinoJson/Memory/StringPool.hpp>
#include <ArduinoJson/Memory/VariantPoolList.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/utility.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class VariantSlot;
class VariantPool;

class ResourceManager {
 public:
  ResourceManager(Allocator* allocator = DefaultAllocator::instance())
      : allocator_(allocator), overflowed_(false) {}

  ~ResourceManager() {
    stringPool_.clear(allocator_);
    variantPools_.clear(allocator_);
  }

  ResourceManager(const ResourceManager&) = delete;
  ResourceManager& operator=(const ResourceManager& src) = delete;

  ResourceManager& operator=(ResourceManager&& src) {
    stringPool_.clear(allocator_);
    variantPools_.clear(allocator_);
    allocator_ = src.allocator_;
    variantPools_ = detail::move(src.variantPools_);
    overflowed_ = src.overflowed_;
    stringPool_ = detail::move(src.stringPool_);
    return *this;
  }

  Allocator* allocator() const {
    return allocator_;
  }

  size_t size() const {
    return VariantPool::slotsToBytes(variantPools_.usage()) +
           stringPool_.size();
  }

  bool overflowed() const {
    return overflowed_;
  }

  SlotWithId allocSlot() {
    auto p = variantPools_.allocSlot(allocator_);
    if (!p)
      overflowed_ = true;
    return p;
  }

  void freeSlot(SlotWithId id) {
    variantPools_.freeSlot(id);
  }

  VariantSlot* getSlot(SlotId id) const {
    return variantPools_.getSlot(id);
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

  void saveString(StringNode* node) {
    stringPool_.add(node);
  }

  template <typename TAdaptedString>
  StringNode* getString(const TAdaptedString& str) const {
    return stringPool_.get(str);
  }

  StringNode* createString(size_t length) {
    auto node = StringNode::create(length, allocator_);
    if (!node)
      overflowed_ = true;
    return node;
  }

  StringNode* resizeString(StringNode* node, size_t length) {
    node = StringNode::resize(node, length, allocator_);
    if (!node)
      overflowed_ = true;
    return node;
  }

  void destroyString(StringNode* node) {
    StringNode::destroy(node, allocator_);
  }

  void dereferenceString(const char* s) {
    stringPool_.dereference(s, allocator_);
  }

  void clear() {
    variantPools_.clear(allocator_);
    overflowed_ = false;
    stringPool_.clear(allocator_);
  }

  void shrinkToFit() {
    variantPools_.shrinkToFit(allocator_);
  }

 private:
  Allocator* allocator_;
  bool overflowed_;
  StringPool stringPool_;
  VariantPoolList variantPools_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
