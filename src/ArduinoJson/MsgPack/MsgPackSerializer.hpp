// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/MsgPack/endianess.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Serialization/CountingDecorator.hpp>
#include <ArduinoJson/Serialization/measure.hpp>
#include <ArduinoJson/Serialization/serialize.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TWriter>
class MsgPackSerializer : public VariantDataVisitor<size_t> {
 public:
  static const bool producesText = false;

  MsgPackSerializer(TWriter writer, const ResourceManager* resources)
      : writer_(writer), resources_(resources) {}

  template <typename T>
  typename enable_if<is_floating_point<T>::value && sizeof(T) == 4,
                     size_t>::type
  visit(T value32) {
    if (canConvertNumber<JsonInteger>(value32)) {
      JsonInteger truncatedValue = JsonInteger(value32);
      if (value32 == T(truncatedValue))
        return visit(truncatedValue);
    }
    writeByte(0xCA);
    writeInteger(value32);
    return bytesWritten();
  }

  template <typename T>
  ARDUINOJSON_NO_SANITIZE("float-cast-overflow")
  typename enable_if<is_floating_point<T>::value && sizeof(T) == 8,
                     size_t>::type visit(T value64) {
    float value32 = float(value64);
    if (value32 == value64)
      return visit(value32);
    writeByte(0xCB);
    writeInteger(value64);
    return bytesWritten();
  }

  size_t visit(const ArrayData& array) {
    size_t n = array.size(resources_);
    if (n < 0x10) {
      writeByte(uint8_t(0x90 + n));
    } else if (n < 0x10000) {
      writeByte(0xDC);
      writeInteger(uint16_t(n));
    } else {
      writeByte(0xDD);
      writeInteger(uint32_t(n));
    }

    auto slotId = array.head();
    while (slotId != NULL_SLOT) {
      auto slot = resources_->getSlot(slotId);
      slot->data()->accept(*this);
      slotId = slot->next();
    }

    return bytesWritten();
  }

  size_t visit(const ObjectData& object) {
    size_t n = object.size(resources_);
    if (n < 0x10) {
      writeByte(uint8_t(0x80 + n));
    } else if (n < 0x10000) {
      writeByte(0xDE);
      writeInteger(uint16_t(n));
    } else {
      writeByte(0xDF);
      writeInteger(uint32_t(n));
    }

    auto slotId = object.head();
    while (slotId != NULL_SLOT) {
      auto slot = resources_->getSlot(slotId);
      visit(slot->key());
      slot->data()->accept(*this);
      slotId = slot->next();
    }

    return bytesWritten();
  }

  size_t visit(const char* value) {
    return visit(JsonString(value));
  }

  size_t visit(JsonString value) {
    ARDUINOJSON_ASSERT(value != NULL);

    auto n = value.size();

    if (n < 0x20) {
      writeByte(uint8_t(0xA0 + n));
    } else if (n < 0x100) {
      writeByte(0xD9);
      writeInteger(uint8_t(n));
    } else if (n < 0x10000) {
      writeByte(0xDA);
      writeInteger(uint16_t(n));
    } else {
      writeByte(0xDB);
      writeInteger(uint32_t(n));
    }
    writeBytes(reinterpret_cast<const uint8_t*>(value.c_str()), n);
    return bytesWritten();
  }

  size_t visit(RawString value) {
    writeBytes(reinterpret_cast<const uint8_t*>(value.data()), value.size());
    return bytesWritten();
  }

  size_t visit(JsonInteger value) {
    if (value > 0) {
      visit(static_cast<JsonUInt>(value));
    } else if (value >= -0x20) {
      writeInteger(int8_t(value));
    } else if (value >= -0x80) {
      writeByte(0xD0);
      writeInteger(int8_t(value));
    } else if (value >= -0x8000) {
      writeByte(0xD1);
      writeInteger(int16_t(value));
    }
#if ARDUINOJSON_USE_LONG_LONG
    else if (value >= -0x80000000LL)
#else
    else
#endif
    {
      writeByte(0xD2);
      writeInteger(int32_t(value));
    }
#if ARDUINOJSON_USE_LONG_LONG
    else {
      writeByte(0xD3);
      writeInteger(int64_t(value));
    }
#endif
    return bytesWritten();
  }

  size_t visit(JsonUInt value) {
    if (value <= 0x7F) {
      writeInteger(uint8_t(value));
    } else if (value <= 0xFF) {
      writeByte(0xCC);
      writeInteger(uint8_t(value));
    } else if (value <= 0xFFFF) {
      writeByte(0xCD);
      writeInteger(uint16_t(value));
    }
#if ARDUINOJSON_USE_LONG_LONG
    else if (value <= 0xFFFFFFFF)
#else
    else
#endif
    {
      writeByte(0xCE);
      writeInteger(uint32_t(value));
    }
#if ARDUINOJSON_USE_LONG_LONG
    else {
      writeByte(0xCF);
      writeInteger(uint64_t(value));
    }
#endif
    return bytesWritten();
  }

  size_t visit(bool value) {
    writeByte(value ? 0xC3 : 0xC2);
    return bytesWritten();
  }

  size_t visit(nullptr_t) {
    writeByte(0xC0);
    return bytesWritten();
  }

 private:
  size_t bytesWritten() const {
    return writer_.count();
  }

  void writeByte(uint8_t c) {
    writer_.write(c);
  }

  void writeBytes(const uint8_t* p, size_t n) {
    writer_.write(p, n);
  }

  template <typename T>
  void writeInteger(T value) {
    fixEndianess(value);
    writeBytes(reinterpret_cast<uint8_t*>(&value), sizeof(value));
  }

  CountingDecorator<TWriter> writer_;
  const ResourceManager* resources_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// Produces a MessagePack document.
// https://arduinojson.org/v7/api/msgpack/serializemsgpack/
template <typename TDestination>
inline size_t serializeMsgPack(JsonVariantConst source, TDestination& output) {
  using namespace ArduinoJson::detail;
  return serialize<MsgPackSerializer>(source, output);
}

// Produces a MessagePack document.
// https://arduinojson.org/v7/api/msgpack/serializemsgpack/
inline size_t serializeMsgPack(JsonVariantConst source, void* output,
                               size_t size) {
  using namespace ArduinoJson::detail;
  return serialize<MsgPackSerializer>(source, output, size);
}

// Computes the length of the document that serializeMsgPack() produces.
// https://arduinojson.org/v7/api/msgpack/measuremsgpack/
inline size_t measureMsgPack(JsonVariantConst source) {
  using namespace ArduinoJson::detail;
  return measure<MsgPackSerializer>(source);
}

ARDUINOJSON_END_PUBLIC_NAMESPACE
