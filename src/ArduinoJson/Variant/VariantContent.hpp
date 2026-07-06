// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <stddef.h>  // size_t

#include <ArduinoJson/Numbers/JsonFloat.hpp>
#include <ArduinoJson/Numbers/JsonInteger.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

enum class VariantTypeBits : uint8_t {
  OwnedStringBit = 0x01,  // 0000 0001
  NumberBit = 0x08,       // 0000 1000
#if ARDUINOJSON_USE_8_BYTE_POOL
  EightByteBit = 0x10,  // 0001 0000
#endif
  CollectionMask = 0x60,
};

enum class VariantType : uint8_t {
  Null = 0,           // 0000 0000
  TinyString = 0x02,  // 0000 0010
  RawString = 0x03,   // 0000 0011
  LongString = 0x05,  // 0000 0101
  Boolean = 0x06,     // 0000 0110
  Uint32 = 0x0A,      // 0000 1010
  Int32 = 0x0C,       // 0000 1100
  Float = 0x0E,       // 0000 1110
#if ARDUINOJSON_USE_LONG_LONG
  Uint64 = 0x1A,  // 0001 1010
  Int64 = 0x1C,   // 0001 1100
#endif
#if ARDUINOJSON_USE_DOUBLE
  Double = 0x1E,  // 0001 1110
#endif
  Object = 0x20,
  Array = 0x40,
};

inline bool operator&(VariantType type, VariantTypeBits bit) {
  return (uint8_t(type) & uint8_t(bit)) != 0;
}

struct CollectionData {
  SlotId head = NULL_SLOT;
  SlotId tail = NULL_SLOT;

  // Placement new
  static void* operator new(size_t, void* p) noexcept {
    return p;
  }

  static void operator delete(void*, void*) noexcept {}
};

const size_t tinyStringMaxLength = 3;

union VariantContent {
  VariantContent() {}

  float asFloat;
  bool asBoolean;
  uint32_t asUint32;
  int32_t asInt32;
#if ARDUINOJSON_USE_8_BYTE_POOL
  SlotId asSlotId;
#endif
  CollectionData asCollection;
  struct StringNode* asStringNode;
  char asTinyString[tinyStringMaxLength + 1];
};

#if ARDUINOJSON_USE_8_BYTE_POOL
union EightByteValue {
#  if ARDUINOJSON_USE_LONG_LONG
  uint64_t asUint64;
  int64_t asInt64;
#  endif
#  if ARDUINOJSON_USE_DOUBLE
  double asDouble;
#  endif
};

static_assert(sizeof(EightByteValue) == 8,
              "sizeof(EightByteValue) must be 8 bytes");
#endif

ARDUINOJSON_END_PRIVATE_NAMESPACE
