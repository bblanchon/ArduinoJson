// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/MsgPack/endianess.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Serialization/CountingDecorator.hpp>
#include <ArduinoJson/Serialization/measure.hpp>
#include <ArduinoJson/Serialization/serialize.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TWriter>
class MsgPackSerializer : public Visitor<size_t> {
 public:
  static const bool producesText = false;

  MsgPackSerializer(TWriter writer) : _writer(writer) {}

  template <typename T>
  typename enable_if<sizeof(T) == 4, size_t>::type visitFloat(T value32) {
    writeByte(0xCA);
    writeInteger(value32);
    return bytesWritten();
  }

  template <typename T>
  ARDUINOJSON_NO_SANITIZE("float-cast-overflow")
  typename enable_if<sizeof(T) == 8, size_t>::type visitFloat(T value64) {
    float value32 = float(value64);
    if (value32 == value64) {
      writeByte(0xCA);
      writeInteger(value32);
    } else {
      writeByte(0xCB);
      writeInteger(value64);
    }
    return bytesWritten();
  }

  size_t visitArray(const CollectionData& array) {
    size_t n = array.size();
    if (n < 0x10) {
      writeByte(uint8_t(0x90 + array.size()));
    } else if (n < 0x10000) {
      writeByte(0xDC);
      writeInteger(uint16_t(n));
    } else {
      writeByte(0xDD);
      writeInteger(uint32_t(n));
    }
    for (VariantSlot* slot = array.head(); slot; slot = slot->next()) {
      slot->data()->accept(*this);
    }
    return bytesWritten();
  }

  size_t visitObject(const CollectionData& object) {
    size_t n = object.size();
    if (n < 0x10) {
      writeByte(uint8_t(0x80 + n));
    } else if (n < 0x10000) {
      writeByte(0xDE);
      writeInteger(uint16_t(n));
    } else {
      writeByte(0xDF);
      writeInteger(uint32_t(n));
    }
    for (VariantSlot* slot = object.head(); slot; slot = slot->next()) {
      visitString(slot->key());
      slot->data()->accept(*this);
    }
    return bytesWritten();
  }

  size_t visitString(const char* value) {
    ARDUINOJSON_ASSERT(value != NULL);

    size_t n = strlen(value);

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
    writeBytes(reinterpret_cast<const uint8_t*>(value), n);
    return bytesWritten();
  }

  size_t visitRawJson(const char* data, size_t size) {
    writeBytes(reinterpret_cast<const uint8_t*>(data), size);
    return bytesWritten();
  }

  size_t visitSignedInteger(Integer value) {
    if (value > 0) {
      visitUnsignedInteger(static_cast<UInt>(value));
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

  size_t visitUnsignedInteger(UInt value) {
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

  size_t visitBoolean(bool value) {
    writeByte(value ? 0xC3 : 0xC2);
    return bytesWritten();
  }

  size_t visitNull() {
    writeByte(0xC0);
    return bytesWritten();
  }

 private:
  size_t bytesWritten() const {
    return _writer.count();
  }

  void writeByte(uint8_t c) {
    _writer.write(c);
  }

  void writeBytes(const uint8_t* p, size_t n) {
    _writer.write(p, n);
  }

  template <typename T>
  void writeInteger(T value) {
    fixEndianess(value);
    writeBytes(reinterpret_cast<uint8_t*>(&value), sizeof(value));
  }

  CountingDecorator<TWriter> _writer;
};

template <typename TSource, typename TDestination>
inline size_t serializeMsgPack(const TSource& source, TDestination& output) {
  return serialize<MsgPackSerializer>(source, output);
}

template <typename TSource>
inline size_t serializeMsgPack(const TSource& source, void* output,
                               size_t size) {
  return serialize<MsgPackSerializer>(source, output, size);
}

template <typename TSource>
inline size_t measureMsgPack(const TSource& source) {
  return measure<MsgPackSerializer>(source);
}

}  // namespace ARDUINOJSON_NAMESPACE
