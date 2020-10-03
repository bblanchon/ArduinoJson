// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Deserialization/deserialize.hpp>
#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/MsgPack/endianess.hpp>
#include <ArduinoJson/MsgPack/ieee754.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TReader, typename TStringStorage>
class MsgPackDeserializer {
 public:
  MsgPackDeserializer(MemoryPool &pool, TReader reader,
                      TStringStorage stringStorage)
      : _pool(&pool),
        _reader(reader),
        _stringStorage(stringStorage),
        _error(DeserializationError::Ok),
        _foundSomething(false) {}

  // TODO: add support for filter
  template <typename TFilter>
  DeserializationError parse(VariantData &variant, TFilter filter,
                             NestingLimit nestingLimit) {
    parseVariant(variant, filter, nestingLimit);
    return _foundSomething ? _error : DeserializationError::EmptyInput;
  }

 private:
  template <typename TFilter>
  bool parseVariant(VariantData &variant, TFilter filter,
                    NestingLimit nestingLimit) {
    uint8_t code;
    if (!readByte(code))
      return false;

    _foundSomething = true;

    if ((code & 0x80) == 0) {
      if (filter.allowValue())
        variant.setUnsignedInteger(code);
      return true;
    }

    if ((code & 0xe0) == 0xe0) {
      if (filter.allowValue())
        variant.setSignedInteger(static_cast<int8_t>(code));
      return true;
    }

    if ((code & 0xe0) == 0xa0) {
      if (filter.allowValue())
        return readString(variant, code & 0x1f);
      else
        return skipString(code & 0x1f);
      
    }

    if ((code & 0xf0) == 0x90) {
      if (filter.allowArray())
        return readArray(variant.toArray(), code & 0x0F, nestingLimit);
      else
        return skipArray(code & 0x0F, nestingLimit);
    }

    if ((code & 0xf0) == 0x80) {
      if (filter.allowObject())
        return readObject(variant.toObject(), code & 0x0F, nestingLimit);
      else
        return skipObject(code & 0x0F, nestingLimit);
      
    }

    switch (code) {
      case 0xc0:
        // already null
        return true;

      case 0xc2:
        if (filter.allowValue())
          variant.setBoolean(false);
        return true;

      case 0xc3:
        if (filter.allowValue())
          variant.setBoolean(true);
        return true;

      case 0xcc:
        if (filter.allowValue())
          return readInteger<uint8_t>(variant);
        else
          return skipInteger<uint8_t>();
        

      case 0xcd:
        if (filter.allowValue())
          return readInteger<uint16_t>(variant);
        else
          return skipInteger<uint16_t>();

      case 0xce:
        if (filter.allowValue())
          return readInteger<uint32_t>(variant);
        else
          return skipInteger<uint32_t>();

#if ARDUINOJSON_USE_LONG_LONG
      case 0xcf:
        if (filter.allowValue())
          return readInteger<uint64_t>(variant);
        else
          return skipInteger<uint64_t>();
#endif

      case 0xd0:
        if (filter.allowValue())
          return readInteger<int8_t>(variant);
        else
          return skipInteger<int8_t>();        

      case 0xd1:
        if (filter.allowValue())
          return readInteger<int16_t>(variant);
        else
          return skipInteger<int16_t>();

      case 0xd2:
        if (filter.allowValue())
          return readInteger<int32_t>(variant);
        else
          return skipInteger<int32_t>();          

#if ARDUINOJSON_USE_LONG_LONG
      case 0xd3:
        if (filter.allowValue())
          return readInteger<int64_t>(variant);
        else
          return skipInteger<int64_t>();             
#endif

      case 0xca:
        if (filter.allowValue())
          return readFloat<float>(variant);
        else
          return skipFloat<float>();         

      case 0xcb:
        if (filter.allowValue())
          return readDouble<double>(variant);
        else
          return skipDouble<double>();                   

      case 0xd9:
        if (filter.allowValue())
          return readString<uint8_t>(variant);
        else
          return skipString<uint8_t>();                   

      case 0xda:
        if (filter.allowValue())
          return readString<uint16_t>(variant);
        else
          return skipString<uint16_t>();                   

      case 0xdb:
        if (filter.allowValue())
          return readString<uint32_t>(variant);
        else
          return skipString<uint32_t>();         

      case 0xdc:
        if (filter.allowArray())
          return readArray<uint16_t>(variant.toArray(), nestingLimit);
        else
          return skipArray<uint16_t>(nestingLimit);
        
      case 0xdd:
        if (filter.allowArray())
          return readArray<uint32_t>(variant.toArray(), nestingLimit);
        else
          return skipArray<uint32_t>(nestingLimit);          

      case 0xde:
        if (filter.allowObject())
          return readObject<uint16_t>(variant.toObject(), nestingLimit);
        else
          return skipObject<uint16_t>(nestingLimit);
        

      case 0xdf:
        if (filter.allowObject())
          return readObject<uint32_t>(variant.toObject(), nestingLimit);
        else
          return skipObject<uint32_t>(nestingLimit);
        

      default:
        _error = DeserializationError::NotSupported;
        return false;
    }
  }

bool skipVariant(NestingLimit nestingLimit) {
    uint8_t code;
    if (!readByte(code))
      return false;

    _foundSomething = true;

    if ((code & 0x80) == 0) {
      return true;
    }

    if ((code & 0xe0) == 0xe0) {
      return true;
    }

    if ((code & 0xe0) == 0xa0) {
      return skipString(code & 0x1f);
    }

    if ((code & 0xf0) == 0x90) {
      return skipArray(code & 0x0F, nestingLimit);
    }

    if ((code & 0xf0) == 0x80) {
      return skipObject(code & 0x0F, nestingLimit);
    }

    switch (code) {
      case 0xc0:
        // already null
        return true;

      case 0xc2:
        return true;

      case 0xc3:
        return true;

      case 0xcc:
        return skipInteger<uint8_t>();

      case 0xcd:
        return skipInteger<uint16_t>();

      case 0xce:
        return skipInteger<uint32_t>();

#if ARDUINOJSON_USE_LONG_LONG
      case 0xcf:
        return skipInteger<uint64_t>();
#endif

      case 0xd0:
        return skipInteger<int8_t>();

      case 0xd1:
        return skipInteger<int16_t>();

      case 0xd2:
        return skipInteger<int32_t>();

#if ARDUINOJSON_USE_LONG_LONG
      case 0xd3:
        return skipInteger<int64_t>();
#endif

      case 0xca:
        return skipFloat<float>();

      case 0xcb:
        return skipDouble<double>();

      case 0xd9:
        return skipString<uint8_t>();

      case 0xda:
        return skipString<uint16_t>();

      case 0xdb:
        return skipString<uint32_t>();

      case 0xdc:
        return skipArray<uint16_t>(nestingLimit);

      case 0xdd:
        return skipArray<uint32_t>(nestingLimit);

      case 0xde:
        return skipObject<uint16_t>(nestingLimit);

      case 0xdf:
        return skipObject<uint32_t>(nestingLimit);

      default:
        _error = DeserializationError::NotSupported;
        return false;
    }
  }

 private:
  // Prevent VS warning "assignment operator could not be generated"
  MsgPackDeserializer &operator=(const MsgPackDeserializer &);

  bool readByte(uint8_t &value) {
    int c = _reader.read();
    if (c < 0) {
      _error = DeserializationError::IncompleteInput;
      return false;
    }
    value = static_cast<uint8_t>(c);
    return true;
  }

  bool readBytes(uint8_t *p, size_t n) {
    if (_reader.readBytes(reinterpret_cast<char *>(p), n) == n)
      return true;
    _error = DeserializationError::IncompleteInput;
    return false;
  }

  bool skipBytes(size_t n) {
    for (; n; --n){
      if (_reader.read() < 0){
        _error = DeserializationError::IncompleteInput;
        return false;
      }
    }
    return true;
  }


  template <typename T>
  bool readBytes(T &value) {
    return readBytes(reinterpret_cast<uint8_t *>(&value), sizeof(value));
  }

  template <typename T>
  bool skipBytes() {
    return skipBytes(sizeof(T));
  }


  template <typename T>
  bool readInteger(T &value) {
    if (!readBytes(value))
      return false;
    fixEndianess(value);
    return true;
  }


  template <typename T>
  bool readInteger(VariantData &variant) {
    T value;
    if (!readInteger(value))
      return false;
    variant.setInteger(value);
    return true;
  }

  template <typename T>
  bool skipInteger() {
    return skipBytes<T>();
  }




  template <typename T>
  typename enable_if<sizeof(T) == 4, bool>::type readFloat(
      VariantData &variant) {
    T value;
    if (!readBytes(value))
      return false;
    fixEndianess(value);
    variant.setFloat(value);
    return true;
  }

  template <typename T>
  typename enable_if<sizeof(T) == 4, bool>::type skipFloat() {
    return skipBytes<T>();
  }


  template <typename T>
  typename enable_if<sizeof(T) == 8, bool>::type readDouble(
      VariantData &variant) {
    T value;
    if (!readBytes(value))
      return false;
    fixEndianess(value);
    variant.setFloat(value);
    return true;
  }


  template <typename T>
  typename enable_if<sizeof(T) == 4, bool>::type readDouble(
      VariantData &variant) {
    uint8_t i[8];  // input is 8 bytes
    T value;       // output is 4 bytes
    uint8_t *o = reinterpret_cast<uint8_t *>(&value);
    if (!readBytes(i, 8))
      return false;
    doubleToFloat(i, o);
    fixEndianess(value);
    variant.setFloat(value);
    return true;
  }


  template <typename T>
  bool skipDouble(){
    return skipBytes(8);
  }


  template <typename T>
  bool readString(VariantData &variant) {
    T size;
    if (!readInteger(size))
      return false;
    return readString(variant, size);
  }

  template <typename T>
  bool readString(const char *&str) {
    T size;
    if (!readInteger(size))
      return false;
    return readString(str, size);
  }

  template <typename T>
  bool skipString() {
    T size;
    if (!readInteger(size))
      return false;
    return skipString(size);
  }

  bool readString(VariantData &variant, size_t n) {
    const char *s = 0;  // <- mute "maybe-uninitialized" (+4 bytes on AVR)
    if (!readString(s, n))
      return false;
    variant.setStringPointer(s, typename TStringStorage::storage_policy());
    return true;
  }

  bool readString(const char *&result, size_t n) {
    _stringStorage.startString();
    for (; n; --n) {
      uint8_t c;
      if (!readBytes(c))
        return false;
      _stringStorage.append(static_cast<char>(c));
    }
    _stringStorage.append('\0');
    if (!_stringStorage.isValid()) {
      _error = DeserializationError::NoMemory;
      return false;
    }

    result = _stringStorage.save();
    return true;
  }

  bool skipString(size_t n) {
    return skipBytes(n);
  }


  template <typename TSize, typename TFilter>
  bool readArray(CollectionData &array, TFilter filter,
                 NestingLimit nestingLimit) {
    TSize size;
    if (!readInteger(size))
      return false;
    return readArray(array, size, filter, nestingLimit);
  }

  template <typename TSize>
  bool skipArray(NestingLimit nestingLimit) {
    TSize size;
    if (!readInteger(size))
      return false;
    return skipArray(size, nestingLimit);
  }

  template <typename TFilter>
  bool readArray(CollectionData &array, size_t n, TFilter filter,
                 NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }

    TFilter memberFilter = filter[0UL];

    for (; n; --n) {
      if (memberFilter.allow()) {
        VariantData *value = array.addElement(_pool);
        if (!value) {
          _error = DeserializationError::NoMemory;
          return false;
        }

        if (!parseVariant(*value, filter, nestingLimit.decrement()))
          return false;
      } else {
        if (!skipVariant(nestingLimit.decrement()))
          return false;
      }
    }

    return true;
  }

  bool skipArray(size_t n, NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }

    for (; n; --n) {
      if (!skipVariant(nestingLimit.decrement()))
        return false;
    }

    return true;
  }


  template <typename TSize, typename TFilter>
  bool readObject(CollectionData &object, TFilter filter,
                  NestingLimit nestingLimit) {
    TSize size;
    if (!readInteger(size))
      return false;
    return readObject(object, size, filter, nestingLimit);
  }

  template <typename TSize>
  bool skipObject(NestingLimit nestingLimit) {
    TSize size;
    if (!readInteger(size))
      return false;
    return skipObject(size, nestingLimit);
  }

  template <typename TFilter>
  bool readObject(CollectionData &object, size_t n, TFilter filter,
                  NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }

    for (; n; --n) {

      const char *key = 0;  // <- mute "maybe-uninitialized" (+4 bytes on AVR)
      
      if (!parseKey(key))
        return false;

      TFilter memberFilter = filter[key];

      if (memberFilter.allow()) {

        VariantSlot *slot = object.addSlot(_pool);
        if (!slot) {
          _error = DeserializationError::NoMemory;
          return false;
        }

        slot->setKey(key, typename TStringStorage::storage_policy());

        if (!parseVariant(*slot->data(), memberFilter, nestingLimit.decrement()))
          return false;

      } else {
        if (!skipVariant(nestingLimit.decrement()))
          return false;
      }
      
    }

    return true;
  }

  bool skipObject(size_t n, NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }

    for (; n; --n) {
      if (!skipKey())
        return false;

      if (!skipVariant(nestingLimit.decrement()))
        return false;
    }

    return true;
  }

  bool parseKey(const char *&key) {
    uint8_t code;
    if (!readByte(code))
      return false;

    if ((code & 0xe0) == 0xa0)
      return readString(key, code & 0x1f);

    switch (code) {
      case 0xd9:
        return readString<uint8_t>(key);

      case 0xda:
        return readString<uint16_t>(key);

      case 0xdb:
        return readString<uint32_t>(key);

      default:
        _error = DeserializationError::NotSupported;
        return false;
    }
  }

  bool skipKey() {
    uint8_t code;
    if (!readByte(code))
      return false;

    if ((code & 0xe0) == 0xa0)
      return skipString(code & 0x1f);

    switch (code) {
      case 0xd9:
        return skipString<uint8_t>(key);

      case 0xda:
        return skipString<uint16_t>(key);

      case 0xdb:
        return skipString<uint32_t>(key);

      default:
        _error = DeserializationError::NotSupported;
        return false;
    }
  }

  MemoryPool *_pool;
  TReader _reader;
  TStringStorage _stringStorage;
  DeserializationError _error;
  bool _foundSomething;
};

template <typename TInput>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, const TInput &input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit,
                                          AllowAllFilter());
}

template <typename TInput>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, TInput *input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit, filter);
}

template <typename TInput>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, TInput *input, size_t inputSize,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, inputSize, nestingLimit,
                                          AllowAllFilter());
}

template <typename TInput>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, TInput &input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit,
                                          AllowAllFilter());
}
}  // namespace ARDUINOJSON_NAMESPACE
