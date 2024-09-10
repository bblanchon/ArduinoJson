// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/Allocator.hpp>
#include <ArduinoJson/Memory/MemoryPoolList.hpp>
#include <ArduinoJson/Memory/StringPool.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/utility.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class VariantData;
class VariantWithId;

class ResourceManager {
  union SlotData {
    VariantData variant;
#if ARDUINOJSON_USE_EXTENSIONS
    VariantExtension extension;
#endif
  };

 public:
  constexpr static size_t slotSize = sizeof(SlotData);

  ResourceManager(Allocator* allocator = DefaultAllocator::instance())
      : allocator_(allocator), overflowed_(false) {}

  ~ResourceManager() {
    stringPool_.clear(allocator_);
    variantPools_.clear(allocator_);
  }

  ResourceManager(const ResourceManager&) = delete;
  ResourceManager& operator=(const ResourceManager& src) = delete;

  friend void swap(ResourceManager& a, ResourceManager& b) {
    swap(a.stringPool_, b.stringPool_);
    swap(a.variantPools_, b.variantPools_);
    swap_(a.allocator_, b.allocator_);
    swap_(a.overflowed_, b.overflowed_);
  }

  Allocator* allocator() const {
    return allocator_;
  }

  size_t size() const {
    return variantPools_.size() + stringPool_.size();
  }

  bool overflowed() const {
    return overflowed_;
  }

  Slot<VariantData> allocVariant();
  void freeVariant(Slot<VariantData> slot);
  VariantData* getVariant(SlotId id) const;

#if ARDUINOJSON_USE_EXTENSIONS
  Slot<VariantExtension> allocExtension();
  void freeExtension(SlotId slot);
  VariantExtension* getExtension(SlotId id) const;
#endif

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
  MemoryPoolList<SlotData> variantPools_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
