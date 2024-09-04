// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Deserialization/deserialize.hpp>
#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Memory/StringBuffer.hpp>
#include <ArduinoJson/MsgPack/endianness.hpp>
#include <ArduinoJson/MsgPack/ieee754.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TReader>
class MsgPackDeserializer {
 public:
  MsgPackDeserializer(ResourceManager* resources, TReader reader)
      : resources_(resources),
        reader_(reader),
        stringBuffer_(resources),
        foundSomething_(false) {}

  template <typename TFilter>
  DeserializationError parse(VariantData& variant, TFilter filter,
                             DeserializationOption::NestingLimit nestingLimit) {
    DeserializationError::Code err;
    err = parseVariant(&variant, filter, nestingLimit);
    return foundSomething_ ? err : DeserializationError::EmptyInput;
  }

 private:
  template <typename TFilter>
  DeserializationError::Code parseVariant(
      VariantData* variant, TFilter filter,
      DeserializationOption::NestingLimit nestingLimit) {
    DeserializationError::Code err;

    uint8_t header[5];
    err = readBytes(header, 1);
    if (err)
      return err;

    const uint8_t& code = header[0];

    foundSomething_ = true;

    bool allowValue = filter.allowValue();

    if (allowValue) {
      // callers pass a null pointer only when value must be ignored
      ARDUINOJSON_ASSERT(variant != 0);
    }

    if (code >= 0xcc && code <= 0xd3) {
      auto width = uint8_t(1U << ((code - 0xcc) % 4));
      if (allowValue)
        return readInteger(variant, width, code >= 0xd0);
      else
        return skipBytes(width);
    }

    switch (code) {
      case 0xc0:
        // already null
        return DeserializationError::Ok;

      case 0xc1:
        return DeserializationError::InvalidInput;

      case 0xc2:
      case 0xc3:
        if (allowValue)
          variant->setBoolean(code == 0xc3);
        return DeserializationError::Ok;

      case 0xca:
        if (allowValue)
          return readFloat<float>(variant);
        else
          return skipBytes(4);

      case 0xcb:
        if (allowValue)
          return readDouble<double>(variant);
        else
          return skipBytes(8);
    }

    if (code <= 0x7f || code >= 0xe0) {  // fixint
      if (allowValue)
        variant->setInteger(static_cast<int8_t>(code), resources_);
      return DeserializationError::Ok;
    }

    uint8_t sizeBytes = 0;
    size_t size = 0;
    bool isExtension = code >= 0xc7 && code <= 0xc9;

    switch (code) {
      case 0xc4:  // bin 8
      case 0xc7:  // ext 8
      case 0xd9:  // str 8
        sizeBytes = 1;
        break;

      case 0xc5:  // bin 16
      case 0xc8:  // ext 16
      case 0xda:  // str 16
      case 0xdc:  // array 16
      case 0xde:  // map 16
        sizeBytes = 2;
        break;

      case 0xc6:  // bin 32
      case 0xc9:  // ext 32
      case 0xdb:  // str 32
      case 0xdd:  // array 32
      case 0xdf:  // map 32
        sizeBytes = 4;
        break;
    }

    if (code >= 0xd4 && code <= 0xd8) {  // fixext
      size = size_t(1) << (code - 0xd4);
      isExtension = true;
    }

    switch (code & 0xf0) {
      case 0x90:  // fixarray
      case 0x80:  // fixmap
        size = code & 0x0F;
        break;
    }

    switch (code & 0xe0) {
      case 0xa0:  // fixstr
        size = code & 0x1f;
        break;
    }

    if (sizeBytes) {
      err = readBytes(header + 1, sizeBytes);
      if (err)
        return err;

      uint32_t size32 = 0;
      for (uint8_t i = 0; i < sizeBytes; i++)
        size32 = (size32 << 8) | header[i + 1];

      size = size_t(size32);
      if (size < size32)                        // integer overflow
        return DeserializationError::NoMemory;  // (not testable on 32/64-bit)
    }

    // array 16, 32 and fixarray
    if (code == 0xdc || code == 0xdd || (code & 0xf0) == 0x90)
      return readArray(variant, size, filter, nestingLimit);

    // map 16, 32 and fixmap
    if (code == 0xde || code == 0xdf || (code & 0xf0) == 0x80)
      return readObject(variant, size, filter, nestingLimit);

    // str 8, 16, 32 and fixstr
    if (code == 0xd9 || code == 0xda || code == 0xdb || (code & 0xe0) == 0xa0) {
      if (allowValue)
        return readString(variant, size);
      else
        return skipBytes(size);
    }

    if (isExtension)
      size++;  // to include the type

    if (allowValue)
      return readRawString(variant, header, uint8_t(1 + sizeBytes), size);
    else
      return skipBytes(size);
  }

  DeserializationError::Code readByte(uint8_t& value) {
    int c = reader_.read();
    if (c < 0)
      return DeserializationError::IncompleteInput;
    value = static_cast<uint8_t>(c);
    return DeserializationError::Ok;
  }

  DeserializationError::Code readBytes(void* p, size_t n) {
    if (reader_.readBytes(reinterpret_cast<char*>(p), n) == n)
      return DeserializationError::Ok;
    return DeserializationError::IncompleteInput;
  }

  template <typename T>
  DeserializationError::Code readBytes(T& value) {
    return readBytes(&value, sizeof(value));
  }

  DeserializationError::Code skipBytes(size_t n) {
    for (; n; --n) {
      if (reader_.read() < 0)
        return DeserializationError::IncompleteInput;
    }
    return DeserializationError::Ok;
  }

  DeserializationError::Code readInteger(VariantData* variant, uint8_t width,
                                         bool isSigned) {
    uint8_t buffer[8];

    auto err = readBytes(buffer, width);
    if (err)
      return err;

    union {
      int64_t signedValue;
      uint64_t unsignedValue;
    };

    if (isSigned)
      signedValue = static_cast<int8_t>(buffer[0]);  // propagate sign bit
    else
      unsignedValue = static_cast<uint8_t>(buffer[0]);

    for (uint8_t i = 1; i < width; i++)
      unsignedValue = (unsignedValue << 8) | buffer[i];

    if (isSigned) {
      auto truncatedValue = static_cast<JsonInteger>(signedValue);
      if (truncatedValue == signedValue) {
        if (!variant->setInteger(truncatedValue, resources_))
          return DeserializationError::NoMemory;
      }
      // else set null on overflow
    } else {
      auto truncatedValue = static_cast<JsonUInt>(unsignedValue);
      if (truncatedValue == unsignedValue)
        if (!variant->setInteger(truncatedValue, resources_))
          return DeserializationError::NoMemory;
      // else set null on overflow
    }

    return DeserializationError::Ok;
  }

  template <typename T>
  enable_if_t<sizeof(T) == 4, DeserializationError::Code> readFloat(
      VariantData* variant) {
    DeserializationError::Code err;
    T value;

    err = readBytes(value);
    if (err)
      return err;

    fixEndianness(value);
    variant->setFloat(value, resources_);

    return DeserializationError::Ok;
  }

  template <typename T>
  enable_if_t<sizeof(T) == 8, DeserializationError::Code> readDouble(
      VariantData* variant) {
    DeserializationError::Code err;
    T value;

    err = readBytes(value);
    if (err)
      return err;

    fixEndianness(value);
    if (variant->setFloat(value, resources_))
      return DeserializationError::Ok;
    else
      return DeserializationError::NoMemory;
  }

  template <typename T>
  enable_if_t<sizeof(T) == 4, DeserializationError::Code> readDouble(
      VariantData* variant) {
    DeserializationError::Code err;
    uint8_t i[8];  // input is 8 bytes
    T value;       // output is 4 bytes
    uint8_t* o = reinterpret_cast<uint8_t*>(&value);

    err = readBytes(i, 8);
    if (err)
      return err;

    doubleToFloat(i, o);
    fixEndianness(value);
    variant->setFloat(value, resources_);

    return DeserializationError::Ok;
  }

  DeserializationError::Code readString(VariantData* variant, size_t n) {
    DeserializationError::Code err;

    err = readString(n);
    if (err)
      return err;

    variant->setOwnedString(stringBuffer_.save());
    return DeserializationError::Ok;
  }

  DeserializationError::Code readString(size_t n) {
    char* p = stringBuffer_.reserve(n);
    if (!p)
      return DeserializationError::NoMemory;

    return readBytes(p, n);
  }

  DeserializationError::Code readRawString(VariantData* variant,
                                           const void* header,
                                           uint8_t headerSize, size_t n) {
    auto totalSize = size_t(headerSize + n);
    if (totalSize < n)                        // integer overflow
      return DeserializationError::NoMemory;  // (not testable on 64-bit)

    char* p = stringBuffer_.reserve(totalSize);
    if (!p)
      return DeserializationError::NoMemory;

    memcpy(p, header, headerSize);

    auto err = readBytes(p + headerSize, n);
    if (err)
      return err;

    variant->setRawString(stringBuffer_.save());
    return DeserializationError::Ok;
  }

  template <typename TFilter>
  DeserializationError::Code readArray(
      VariantData* variant, size_t n, TFilter filter,
      DeserializationOption::NestingLimit nestingLimit) {
    DeserializationError::Code err;

    if (nestingLimit.reached())
      return DeserializationError::TooDeep;

    bool allowArray = filter.allowArray();

    ArrayData* array;
    if (allowArray) {
      ARDUINOJSON_ASSERT(variant != 0);
      array = &variant->toArray();
    } else {
      array = 0;
    }

    TFilter elementFilter = filter[0U];

    for (; n; --n) {
      VariantData* value;

      if (elementFilter.allow()) {
        ARDUINOJSON_ASSERT(array != 0);
        value = array->addElement(resources_);
        if (!value)
          return DeserializationError::NoMemory;
      } else {
        value = 0;
      }

      err = parseVariant(value, elementFilter, nestingLimit.decrement());
      if (err)
        return err;
    }

    return DeserializationError::Ok;
  }

  template <typename TFilter>
  DeserializationError::Code readObject(
      VariantData* variant, size_t n, TFilter filter,
      DeserializationOption::NestingLimit nestingLimit) {
    DeserializationError::Code err;

    if (nestingLimit.reached())
      return DeserializationError::TooDeep;

    ObjectData* object;
    if (filter.allowObject()) {
      ARDUINOJSON_ASSERT(variant != 0);
      object = &variant->toObject();
    } else {
      object = 0;
    }

    for (; n; --n) {
      err = readKey();
      if (err)
        return err;

      JsonString key = stringBuffer_.str();
      TFilter memberFilter = filter[key.c_str()];
      VariantData* member;

      if (memberFilter.allow()) {
        ARDUINOJSON_ASSERT(object != 0);

        // Save key in memory pool.
        auto savedKey = stringBuffer_.save();

        member = object->addMember(savedKey, resources_);
        if (!member)
          return DeserializationError::NoMemory;
      } else {
        member = 0;
      }

      err = parseVariant(member, memberFilter, nestingLimit.decrement());
      if (err)
        return err;
    }

    return DeserializationError::Ok;
  }

  DeserializationError::Code readKey() {
    DeserializationError::Code err;
    uint8_t code;

    err = readByte(code);
    if (err)
      return err;

    if ((code & 0xe0) == 0xa0)
      return readString(code & 0x1f);

    if (code >= 0xd9 && code <= 0xdb) {
      uint8_t sizeBytes = uint8_t(1U << (code - 0xd9));
      uint32_t size = 0;
      for (uint8_t i = 0; i < sizeBytes; i++) {
        err = readByte(code);
        if (err)
          return err;
        size = (size << 8) | code;
      }
      return readString(size);
    }

    return DeserializationError::InvalidInput;
  }

  ResourceManager* resources_;
  TReader reader_;
  StringBuffer stringBuffer_;
  bool foundSomething_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// Parses a MessagePack input and puts the result in a JsonDocument.
// https://arduinojson.org/v7/api/msgpack/deserializemsgpack/
template <typename TDestination, typename... Args>
detail::enable_if_t<detail::is_deserialize_destination<TDestination>::value,
                    DeserializationError>
deserializeMsgPack(TDestination&& dst, Args&&... args) {
  using namespace detail;
  return deserialize<MsgPackDeserializer>(detail::forward<TDestination>(dst),
                                          detail::forward<Args>(args)...);
}

// Parses a MessagePack input and puts the result in a JsonDocument.
// https://arduinojson.org/v7/api/msgpack/deserializemsgpack/
template <typename TDestination, typename TChar, typename... Args>
detail::enable_if_t<detail::is_deserialize_destination<TDestination>::value,
                    DeserializationError>
deserializeMsgPack(TDestination&& dst, TChar* input, Args&&... args) {
  using namespace detail;
  return deserialize<MsgPackDeserializer>(detail::forward<TDestination>(dst),
                                          input,
                                          detail::forward<Args>(args)...);
}

ARDUINOJSON_END_PUBLIC_NAMESPACE
