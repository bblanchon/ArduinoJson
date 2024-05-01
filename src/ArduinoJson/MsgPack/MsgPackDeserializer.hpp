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

    uint8_t code = 0;  // TODO: why do we need to initialize this variable?
    err = readByte(code);
    if (err)
      return err;

    foundSomething_ = true;

    bool allowValue = filter.allowValue();

    if (allowValue) {
      // callers pass a null pointer only when value must be ignored
      ARDUINOJSON_ASSERT(variant != 0);
    }

    size_t size = 0;

    switch (code) {
      case 0xc4:  // bin 8
      case 0xc7:  // ext 8
      case 0xd9:  // str 8
        uint8_t size8;
        err = readInteger(size8);
        size = size8;
        break;

      case 0xc5:  // bin 16
      case 0xc8:  // ext 16
      case 0xda:  // str 16
      case 0xdc:  // array 16
      case 0xde:  // map 16
        uint16_t size16;
        err = readInteger(size16);
        size = size16;
        break;

      case 0xc6:  // bin 32
      case 0xc9:  // ext 32
      case 0xdb:  // str 32
      case 0xdd:  // array 32
      case 0xdf:  // map 32
        uint32_t size32;
        err = readInteger(size32);
        size = size32;
        break;
    }

    if (err)
      return err;

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

    // bin 8, 16, 32
    if (code == 0xc4 || code == 0xc5 || code == 0xc6) {
      if (allowValue)
        return readBinary(variant, size);
      else
        return skipBytes(size);
    }

    // ext 8, 16, 32
    if (code == 0xc7 || code == 0xc8 || code == 0xc9)
      return skipBytes(size + 1);

    switch (code) {
      case 0xc0:
        // already null
        return DeserializationError::Ok;

      case 0xc1:
        return DeserializationError::InvalidInput;

      case 0xc2:
        if (allowValue)
          variant->setBoolean(false);
        return DeserializationError::Ok;

      case 0xc3:
        if (allowValue)
          variant->setBoolean(true);
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

      case 0xcc:
        if (allowValue)
          return readInteger<uint8_t>(variant);
        else
          return skipBytes(1);

      case 0xcd:
        if (allowValue)
          return readInteger<uint16_t>(variant);
        else
          return skipBytes(2);

      case 0xce:
        if (allowValue)
          return readInteger<uint32_t>(variant);
        else
          return skipBytes(4);

      case 0xcf:
#if ARDUINOJSON_USE_LONG_LONG
        if (allowValue)
          return readInteger<uint64_t>(variant);
        else
          return skipBytes(8);
#else
        return skipBytes(8);  // not supported
#endif

      case 0xd0:
        if (allowValue)
          return readInteger<int8_t>(variant);
        else
          return skipBytes(1);

      case 0xd1:
        if (allowValue)
          return readInteger<int16_t>(variant);
        else
          return skipBytes(2);

      case 0xd2:
        if (allowValue)
          return readInteger<int32_t>(variant);
        else
          return skipBytes(4);

      case 0xd3:
#if ARDUINOJSON_USE_LONG_LONG
        if (allowValue)
          return readInteger<int64_t>(variant);
        else
          return skipBytes(8);  // not supported
#else
        return skipBytes(8);
#endif

      case 0xd4:  // fixext 1 (not supported)
        return skipBytes(2);

      case 0xd5:  // fixext 2 (not supported)
        return skipBytes(3);

      case 0xd6:  // fixext 4 (not supported)
        return skipBytes(5);

      case 0xd7:  // fixext 8 (not supported)
        return skipBytes(9);

      case 0xd8:  // fixext 16 (not supported)
        return skipBytes(17);

      default:  // fixint
        if (allowValue)
          variant->setInteger(static_cast<int8_t>(code));
        return DeserializationError::Ok;
    }
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

  template <typename T>
  DeserializationError::Code readInteger(T& value) {
    DeserializationError::Code err;

    err = readBytes(value);
    if (err)
      return err;

    fixEndianness(value);

    return DeserializationError::Ok;
  }

  template <typename T>
  DeserializationError::Code readInteger(VariantData* variant) {
    DeserializationError::Code err;
    T value;

    err = readInteger(value);
    if (err)
      return err;

    variant->setInteger(value);

    return DeserializationError::Ok;
  }

  template <typename T>
  typename enable_if<sizeof(T) == 4, DeserializationError::Code>::type
  readFloat(VariantData* variant) {
    DeserializationError::Code err;
    T value;

    err = readBytes(value);
    if (err)
      return err;

    fixEndianness(value);
    variant->setFloat(value);

    return DeserializationError::Ok;
  }

  template <typename T>
  typename enable_if<sizeof(T) == 8, DeserializationError::Code>::type
  readDouble(VariantData* variant) {
    DeserializationError::Code err;
    T value;

    err = readBytes(value);
    if (err)
      return err;

    fixEndianness(value);
    variant->setFloat(value);

    return DeserializationError::Ok;
  }

  template <typename T>
  typename enable_if<sizeof(T) == 4, DeserializationError::Code>::type
  readDouble(VariantData* variant) {
    DeserializationError::Code err;
    uint8_t i[8];  // input is 8 bytes
    T value;       // output is 4 bytes
    uint8_t* o = reinterpret_cast<uint8_t*>(&value);

    err = readBytes(i, 8);
    if (err)
      return err;

    doubleToFloat(i, o);
    fixEndianness(value);
    variant->setFloat(value);

    return DeserializationError::Ok;
  }

  template <typename T>
  DeserializationError::Code readString() {
    DeserializationError::Code err;
    T size;

    err = readInteger(size);
    if (err)
      return err;

    return readString(size);
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

  DeserializationError::Code readBinary(VariantData* variant, size_t n) {
    uint8_t headerSize;

    if (n <= 0xFF) {
      headerSize = 2;
    }
#if ARDUINOJSON_STRING_LENGTH_SIZE >= 2
    else if (n <= 0xFFFF) {
      headerSize = 3;
    }
#endif
#if ARDUINOJSON_STRING_LENGTH_SIZE >= 4
    else {
      headerSize = 5;
    }
#else
    else {
      return DeserializationError::NoMemory;
    }
#endif

    char* p = stringBuffer_.reserve(headerSize + n);
    if (!p)
      return DeserializationError::NoMemory;

    if (n <= 0xFF) {
      *p++ = '\xc4';
      *p++ = char(n & 0xFF);
    } else if (n <= 0xFFFF) {
      *p++ = '\xc5';
      *p++ = char(n >> 8 & 0xFF);
      *p++ = char(n & 0xFF);
    } else {
      *p++ = '\xc6';
      *p++ = char(n >> 24 & 0xFF);
      *p++ = char(n >> 16 & 0xFF);
      *p++ = char(n >> 8 & 0xFF);
      *p++ = char(n & 0xFF);
    }

    auto err = readBytes(p, n);
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

    switch (code) {
      case 0xd9:
        return readString<uint8_t>();

      case 0xda:
        return readString<uint16_t>();

      case 0xdb:
        return readString<uint32_t>();

      default:
        return DeserializationError::InvalidInput;
    }
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
typename detail::enable_if<
    detail::is_deserialize_destination<TDestination>::value,
    DeserializationError>::type
deserializeMsgPack(TDestination&& dst, Args&&... args) {
  using namespace detail;
  return deserialize<MsgPackDeserializer>(detail::forward<TDestination>(dst),
                                          detail::forward<Args>(args)...);
}

// Parses a MessagePack input and puts the result in a JsonDocument.
// https://arduinojson.org/v7/api/msgpack/deserializemsgpack/
template <typename TDestination, typename TChar, typename... Args>
typename detail::enable_if<
    detail::is_deserialize_destination<TDestination>::value,
    DeserializationError>::type
deserializeMsgPack(TDestination&& dst, TChar* input, Args&&... args) {
  using namespace detail;
  return deserialize<MsgPackDeserializer>(detail::forward<TDestination>(dst),
                                          input,
                                          detail::forward<Args>(args)...);
}

ARDUINOJSON_END_PUBLIC_NAMESPACE
