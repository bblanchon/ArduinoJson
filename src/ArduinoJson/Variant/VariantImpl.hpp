// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionIterator.hpp>
#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Misc/SerializedValue.hpp>
#include <ArduinoJson/Numbers/convertNumber.hpp>
#include <ArduinoJson/Strings/JsonString.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

// HACK: large functions are implemented in static function to give opportunity
// to the compiler to optimize the `this` pointer away.
class VariantImpl {
 public:
  using iterator = CollectionIterator;

  VariantImpl() : data_(nullptr), resources_(nullptr) {}

  VariantImpl(VariantData* data, ResourceManager* resources)
      : data_(data), resources_(resources) {}

  VariantData* data() const {
    return data_;
  }

  ResourceManager* resources() const {
    return resources_;
  }

  template <typename TVisitor>
  typename TVisitor::result_type accept(TVisitor& visit) {
    return accept(visit, data_, resources_);
  }

  template <typename TVisitor>
  static typename TVisitor::result_type accept(TVisitor& visit,
                                               VariantData* data,
                                               ResourceManager* resources) {
    if (!data)
      return visit.visit(nullptr);

#if ARDUINOJSON_USE_8_BYTE_POOL
    auto eightByteValue = getEightByte(data, resources);
#endif
    switch (data->type) {
      case VariantType::Float:
        return visit.visit(data->content.asFloat);

#if ARDUINOJSON_USE_DOUBLE
      case VariantType::Double:
        return visit.visit(eightByteValue->asDouble);
#endif

      case VariantType::Array:
        return visit.visitArray(data);

      case VariantType::Object:
        return visit.visitObject(data);

      case VariantType::TinyString:
        return visit.visit(JsonString(data->content.asTinyString));

      case VariantType::LongString:
        return visit.visit(JsonString(data->content.asStringNode->data,
                                      data->content.asStringNode->length));

      case VariantType::RawString:
        return visit.visit(RawString(data->content.asStringNode->data,
                                     data->content.asStringNode->length));

      case VariantType::Int32:
        return visit.visit(static_cast<JsonInteger>(data->content.asInt32));

      case VariantType::Uint32:
        return visit.visit(static_cast<JsonUInt>(data->content.asUint32));

#if ARDUINOJSON_USE_LONG_LONG
      case VariantType::Int64:
        return visit.visit(eightByteValue->asInt64);

      case VariantType::Uint64:
        return visit.visit(eightByteValue->asUint64);
#endif

      case VariantType::Boolean:
        return visit.visit(data->content.asBoolean != 0);

      default:
        return visit.visit(nullptr);
    }
  }

  VariantData* addNewElement() {
    if (!isArray())
      return nullptr;
    return addNewElement(data_, resources_);
  }

  static VariantData* addNewElement(VariantData*, ResourceManager*);

  static void addElement(Slot<VariantData> slot, VariantData*,
                         ResourceManager*);

  template <typename TAdaptedString>
  VariantData* addMember(TAdaptedString key) {
    if (!isObject())
      return nullptr;
    return addMember(key, data_, resources_);
  }

  template <typename TAdaptedString>
  static VariantData* addMember(TAdaptedString key, VariantData*,
                                ResourceManager*);

  VariantData* addPair(VariantData** value) {
    if (isNull())
      return nullptr;
    return addPair(value, data_, resources_);
  }

  static VariantData* addPair(VariantData** value, VariantData*,
                              ResourceManager*);

  bool asBoolean() const {
    return asBoolean(data_, resources_);
  }

  static bool asBoolean(VariantData* data, ResourceManager* resources) {
    if (!data)
      return false;

#if ARDUINOJSON_USE_8_BYTE_POOL
    auto eightByteValue = getEightByte(data, resources);
#endif
    switch (data->type) {
      case VariantType::Boolean:
        return data->content.asBoolean;
      case VariantType::Uint32:
      case VariantType::Int32:
        return data->content.asUint32 != 0;
      case VariantType::Float:
        return data->content.asFloat != 0;
#if ARDUINOJSON_USE_DOUBLE
      case VariantType::Double:
        return eightByteValue->asDouble != 0;
#endif
      case VariantType::Null:
        return false;
#if ARDUINOJSON_USE_LONG_LONG
      case VariantType::Uint64:
      case VariantType::Int64:
        return eightByteValue->asUint64 != 0;
#endif
      default:
        return true;
    }
  }

  template <typename T>
  T asFloat() const {
    return asFloat<T>(data_, resources_);
  }

  template <typename T>
  static T asFloat(VariantData* data, ResourceManager* resources) {
    if (!data)
      return 0.0;

    static_assert(is_floating_point<T>::value, "T must be a floating point");
#if ARDUINOJSON_USE_8_BYTE_POOL
    auto eightByteValue = getEightByte(data, resources);
#endif
    const char* str = nullptr;
    switch (data->type) {
      case VariantType::Boolean:
        return static_cast<T>(data->content.asBoolean);
      case VariantType::Uint32:
        return static_cast<T>(data->content.asUint32);
      case VariantType::Int32:
        return static_cast<T>(data->content.asInt32);
#if ARDUINOJSON_USE_LONG_LONG
      case VariantType::Uint64:
        return static_cast<T>(eightByteValue->asUint64);
      case VariantType::Int64:
        return static_cast<T>(eightByteValue->asInt64);
#endif
      case VariantType::TinyString:
        str = data->content.asTinyString;
        break;
      case VariantType::LongString:
        str = data->content.asStringNode->data;
        break;
      case VariantType::Float:
        return static_cast<T>(data->content.asFloat);
#if ARDUINOJSON_USE_DOUBLE
      case VariantType::Double:
        return static_cast<T>(eightByteValue->asDouble);
#endif
      default:
        return 0.0;
    }

    ARDUINOJSON_ASSERT(str != nullptr);
    return parseNumber<T>(str);
  }

  template <typename T>
  T asIntegral() const {
    return asIntegral<T>(data_, resources_);
  }

  template <typename T>
  static T asIntegral(VariantData* data, ResourceManager* resources) {
    if (!data)
      return 0;

    static_assert(is_integral<T>::value, "T must be an integral type");
#if ARDUINOJSON_USE_8_BYTE_POOL
    auto eightByteValue = getEightByte(data, resources);
#endif
    const char* str = nullptr;
    switch (data->type) {
      case VariantType::Boolean:
        return data->content.asBoolean;
      case VariantType::Uint32:
        return convertNumber<T>(data->content.asUint32);
      case VariantType::Int32:
        return convertNumber<T>(data->content.asInt32);
#if ARDUINOJSON_USE_LONG_LONG
      case VariantType::Uint64:
        return convertNumber<T>(eightByteValue->asUint64);
      case VariantType::Int64:
        return convertNumber<T>(eightByteValue->asInt64);
#endif
      case VariantType::TinyString:
        str = data->content.asTinyString;
        break;
      case VariantType::LongString:
        str = data->content.asStringNode->data;
        break;
      case VariantType::Float:
        return convertNumber<T>(data->content.asFloat);
#if ARDUINOJSON_USE_DOUBLE
      case VariantType::Double:
        return convertNumber<T>(eightByteValue->asDouble);
#endif
      default:
        return 0;
    }

    ARDUINOJSON_ASSERT(str != nullptr);
    return parseNumber<T>(str);
  }

  iterator at(size_t index) const;

  iterator createIterator() const {
    if (!isCollection())
      return iterator();
    return createIterator(data_, resources_);
  }

  static iterator createIterator(VariantData*, ResourceManager*);

#if ARDUINOJSON_USE_8_BYTE_POOL
  static const EightByteValue* getEightByte(VariantData* data,
                                            ResourceManager* resources) {
    ARDUINOJSON_ASSERT(data != nullptr);
    ARDUINOJSON_ASSERT(resources != nullptr);
    return data->type & VariantTypeBits::EightByteBit
               ? resources->getEightByte(data->content.asSlotId)
               : 0;
  }
#endif

  VariantData* getOrAddElement(size_t index);

  VariantData* getElement(size_t index) const;

  template <typename TAdaptedString>
  VariantData* getMember(TAdaptedString key) const {
    if (!isObject())
      return nullptr;
    return getMember(key, data_, resources_);
  }

  template <typename TAdaptedString>
  static VariantData* getMember(TAdaptedString key, VariantData*,
                                ResourceManager*);

  template <typename TAdaptedString>
  VariantData* getOrAddMember(TAdaptedString key) {
    if (!isObject())
      return nullptr;
    return getOrAddMember(key, data_, resources_);
  }

  template <typename TAdaptedString>
  static VariantData* getOrAddMember(TAdaptedString key, VariantData*,
                                     ResourceManager*);

  bool isArray() const {
    return type() == VariantType::Array;
  }

  bool isCollection() const {
    return type() & VariantTypeBits::CollectionMask;
  }

  template <typename T>
  bool isInteger() const {
    return isInteger<T>(data_, resources_);
  }

  template <typename T>
  static bool isInteger(VariantData* data, ResourceManager* resources) {
    if (!data)
      return false;

#if ARDUINOJSON_USE_LONG_LONG
    auto eightByteValue = getEightByte(data, resources);
#else
    (void)resources;
#endif
    switch (data->type) {
      case VariantType::Uint32:
        return canConvertNumber<T>(data->content.asUint32);

      case VariantType::Int32:
        return canConvertNumber<T>(data->content.asInt32);

#if ARDUINOJSON_USE_LONG_LONG
      case VariantType::Uint64:
        return canConvertNumber<T>(eightByteValue->asUint64);

      case VariantType::Int64:
        return canConvertNumber<T>(eightByteValue->asInt64);
#endif

      default:
        return false;
    }
  }

  bool isNull() const {
    return type() == VariantType::Null;
  }

  bool isObject() const {
    return type() == VariantType::Object;
  }

  size_t nesting() const;

  void removeElement(size_t index);

  void removeElement(CollectionIterator it) {
    removeOne(it);
  }

  template <typename TAdaptedString>
  void removeMember(TAdaptedString key) {
    removePair(findKey(key));
  }

  void removeMember(CollectionIterator it) {
    removePair(it);
  }

  bool setBoolean(bool value) {
    if (!data_)
      return false;
    clear(data_, resources_);
    data_->setBoolean(value);
    return true;
  }

  template <typename T>
  bool setFloat(T value) {
    if (!data_)
      return false;
    clear(data_, resources_);
    return setFloat(value, data_, resources_);
  }

  template <typename T>
  static enable_if_t<sizeof(T) == 4, bool> setFloat(T value, VariantData* data,
                                                    ResourceManager*) {
    ARDUINOJSON_ASSERT(data != nullptr);
    ARDUINOJSON_ASSERT(data->type == VariantType::Null);
    data->type = VariantType::Float;
    data->content.asFloat = value;
    return true;
  }

  template <typename T>
  static enable_if_t<sizeof(T) == 8, bool> setFloat(
      T value, VariantData* data, ResourceManager* resources) {
    ARDUINOJSON_ASSERT(data != nullptr);
    ARDUINOJSON_ASSERT(data->type == VariantType::Null);
    ARDUINOJSON_ASSERT(resources != nullptr);

    float valueAsFloat = static_cast<float>(value);

#if ARDUINOJSON_USE_DOUBLE
    if (value == valueAsFloat) {
      data->type = VariantType::Float;
      data->content.asFloat = valueAsFloat;
    } else {
      auto slot = resources->allocEightByte();
      if (!slot)
        return false;
      data->type = VariantType::Double;
      data->content.asSlotId = slot.id();
      slot->asDouble = value;
    }
#else
    data->type = VariantType::Float;
    data->content.asFloat = valueAsFloat;
#endif
    return true;
  }

  template <typename T>
  bool setInteger(T value) {
    if (!data_)
      return false;
    clear(data_, resources_);
    return setInteger(value, data_, resources_);
  }

  template <typename T>
  static enable_if_t<is_signed<T>::value, bool> setInteger(
      T value, VariantData* data, ResourceManager* resources) {
    ARDUINOJSON_ASSERT(data != nullptr);
    ARDUINOJSON_ASSERT(data->type == VariantType::Null);
    ARDUINOJSON_ASSERT(resources != nullptr);

    if (canConvertNumber<int32_t>(value)) {
      data->type = VariantType::Int32;
      data->content.asInt32 = static_cast<int32_t>(value);
    }
#if ARDUINOJSON_USE_LONG_LONG
    else {
      auto slot = resources->allocEightByte();
      if (!slot)
        return false;
      data->type = VariantType::Int64;
      data->content.asSlotId = slot.id();
      slot->asInt64 = value;
    }
#else
    (void)resources;
#endif
    return true;
  }

  template <typename T>
  static enable_if_t<is_unsigned<T>::value, bool> setInteger(
      T value, VariantData* data, ResourceManager* resources) {
    ARDUINOJSON_ASSERT(data != nullptr);
    ARDUINOJSON_ASSERT(data->type == VariantType::Null);
    ARDUINOJSON_ASSERT(resources != nullptr);

    if (canConvertNumber<uint32_t>(value)) {
      data->type = VariantType::Uint32;
      data->content.asUint32 = static_cast<uint32_t>(value);
    }
#if ARDUINOJSON_USE_LONG_LONG
    else {
      auto slot = resources->allocEightByte();
      if (!slot)
        return false;
      data->type = VariantType::Uint64;
      data->content.asSlotId = slot.id();
      slot->asUint64 = value;
    }
#else
    (void)resources;
#endif
    return true;
  }

  template <typename TAdaptedString>
  bool setRawString(TAdaptedString value) {
    if (!data_)
      return false;
    clear(data_, resources_);
    auto dup = resources_->saveString(adaptString(value.data(), value.size()));
    if (!dup)
      return false;
    data_->setRawString(dup);
    return true;
  }

  template <typename TAdaptedString>
  bool setString(TAdaptedString value) {
    if (!data_)
      return false;
    clear(data_, resources_);
    return setString(value, data_, resources_);
  }

  template <typename TAdaptedString>
  static bool setString(TAdaptedString value, VariantData* data,
                        ResourceManager* resources) {
    ARDUINOJSON_ASSERT(data != nullptr);
    ARDUINOJSON_ASSERT(data->type == VariantType::Null);
    ARDUINOJSON_ASSERT(resources != nullptr);

    if (value.isNull())
      return true;  // TODO: should this be moved up to the member function?

    if (isTinyString(value, value.size())) {
      data->setTinyString(value);
      return true;
    }

    auto dup = resources->saveString(value);
    if (dup) {
      data->setLongString(dup);
      return true;
    }

    return false;
  }

  size_t size() const {
    if (!isCollection())
      return 0;

    return size(data_, resources_);
  }

  static size_t size(VariantData* data, ResourceManager* resources) {
    ARDUINOJSON_ASSERT(data != nullptr);
    ARDUINOJSON_ASSERT(data->isCollection());
    ARDUINOJSON_ASSERT(resources != nullptr);

    size_t n = 0;
    for (auto it = createIterator(data, resources); !it.done();
         it.move(resources))
      n++;

    if (data->type == VariantType::Object) {
      ARDUINOJSON_ASSERT((n % 2) == 0);
      n /= 2;
    }

    return n;
  }

  bool toArray() {
    if (!data_)
      return false;
    clear(data_, resources_);
    data_->toArray();
    return true;
  }

  bool toObject() {
    if (!data_)
      return false;
    clear(data_, resources_);
    data_->toObject();
    return true;
  }

  VariantType type() const {
    return data_ ? data_->type : VariantType::Null;
  }

  // Release the resources used by this variant and set it to null.
  bool clear() {
    if (!data_)
      return false;
    clear(data_, resources_);
    return true;
  }

  static void clear(VariantData* data, ResourceManager* resources) {
    ARDUINOJSON_ASSERT(data != nullptr);
    ARDUINOJSON_ASSERT(resources != nullptr);

    if (data->type & VariantTypeBits::OwnedStringBit)
      resources->dereferenceString(data->content.asStringNode->data);

#if ARDUINOJSON_USE_8_BYTE_POOL
    if (data->type & VariantTypeBits::EightByteBit)
      resources->freeEightByte(data->content.asSlotId);
#endif

    if (data->type & VariantTypeBits::CollectionMask)
      empty(data, resources);

    data->type = VariantType::Null;
  }

  void empty() {
    if (!isCollection())
      return;
    empty(data_, resources_);
  }

  static void empty(VariantData*, ResourceManager*);

  static void freeVariant(Slot<VariantData> slot, ResourceManager* resources) {
    clear(slot.ptr(), resources);
    resources->freeVariant(slot);
  }

 private:
  VariantData* data_;
  ResourceManager* resources_;

  template <typename TAdaptedString>
  iterator findKey(TAdaptedString key) const {
    if (!isObject())
      return iterator();
    return findKey(key, data_, resources_);
  }

  template <typename TAdaptedString>
  static iterator findKey(TAdaptedString key, VariantData*, ResourceManager*);

  static void appendPair(Slot<VariantData> key, Slot<VariantData> value,
                         VariantData*, ResourceManager*);

  void removeOne(iterator it);
  void removePair(iterator it);

  Slot<VariantData> getPreviousSlot(VariantData*) const;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
