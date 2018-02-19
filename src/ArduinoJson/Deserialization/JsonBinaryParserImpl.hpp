// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once


#include "JsonBinaryParser.hpp"


template<typename TReader, typename TWriter>
inline bool
ArduinoJson::Internals::JsonBinaryParser<TReader, TWriter>::parseAnythingTo(
        JsonVariant *destination) {
    if (_nestingLimit == 0) return false;
    _nestingLimit--;
    bool success = parseAnythingToUnsafe(destination);
    _nestingLimit++;
    destination->setParserType(PARSER_TYPE_MESSAGEPACK);
    return success;
}

template<typename TReader, typename TWriter>
inline bool
ArduinoJson::Internals::JsonBinaryParser<TReader, TWriter>::parseAnythingToUnsafe(
        JsonVariant *destination) {
    uint8_t value = _reader.current();

    if (value == MESSAGEPACK_ARRAY_16 || (value >= MESSAGEPACK_ARRAY_FIX_MIN && value <= MESSAGEPACK_ARRAY_FIX_MAX))
        return parseArrayTo(destination);
    else if (value == MESSAGEPACK_MAP_16 || (value >= MESSAGEPACK_MAP_FIX_MIN && value <= MESSAGEPACK_MAP_FIX_MAX))
        return parseObjectTo(destination);
    else
        return parseStringTo(destination);
}

template<typename TReader, typename TWriter>
inline ArduinoJson::JsonArray &
ArduinoJson::Internals::JsonBinaryParser<TReader, TWriter>::parseArray() {
    // Create an empty array
    union {
        uint8_t value[sizeof(uint16_t)];
        uint16_t arraySize;
    } arrayUnion;
    // Create an empty array
    JsonArray &array = _buffer->createArray();

    uint16_t arraySize = 0;

    uint8_t arrayType = _reader.current();

    if (arrayType >= MESSAGEPACK_ARRAY_FIX_MIN && arrayType <= MESSAGEPACK_ARRAY_FIX_MAX) {
        arraySize = arrayType - MESSAGEPACK_ARRAY_FIX_MIN;
        _reader.move();
    } else if (arrayType == MESSAGEPACK_ARRAY_16) {
        _reader.move();
        arrayUnion.value[1] = _reader.current();
        _reader.move();
        arrayUnion.value[0] = _reader.current();
        arraySize = arrayUnion.arraySize;
        _reader.move();
    }


    // Read each value
    while (arraySize--) {
        // 1 - Parse value
        JsonVariant value;
        if (!parseAnythingTo(&value)) goto ERROR_INVALID_VALUE;
        // 2 - Add value to array
        if (!array.add(value)) goto ERROR_NO_MEMORY;
    }

    SUCCESS_EMPTY_ARRAY:
    SUCCES_NON_EMPTY_ARRAY:
    return array;

    ERROR_INVALID_VALUE:
    ERROR_MISSING_BRACKET:
    ERROR_MISSING_COMMA:
    ERROR_NO_MEMORY:
    return JsonArray::invalid();
}

template<typename TReader, typename TWriter>
inline bool ArduinoJson::Internals::JsonBinaryParser<TReader, TWriter>::parseArrayTo(
        JsonVariant *destination) {
    JsonArray &array = parseArray();
    if (!array.success()) return false;

    *destination = array;
    return true;
}

template<typename TReader, typename TWriter>
inline ArduinoJson::JsonObject &
ArduinoJson::Internals::JsonBinaryParser<TReader, TWriter>::parseObject() {
    // Create an empty object
    union {
        uint8_t value[sizeof(uint16_t)];
        uint16_t objectSize;
    } objectUnion;
    // Create an empty object
    JsonObject &object = _buffer->createObject();

    uint16_t objectSize = 0;

    uint8_t objectType = _reader.current();

    if (objectType >= MESSAGEPACK_MAP_FIX_MIN && objectType <= MESSAGEPACK_MAP_FIX_MAX) {
        objectSize = objectType - MESSAGEPACK_MAP_FIX_MIN;
        _reader.move();
    } else if (objectType == MESSAGEPACK_MAP_16) {
        _reader.move();
        objectUnion.value[1] = _reader.current();
        _reader.move();
        objectUnion.value[0] = _reader.current();
        objectSize = objectUnion.objectSize;
        _reader.move();
    }
    // Read each key value pair
    while (objectSize--) {
        // 1 - Parse key
        const char *key = parseString();
        if (!key) goto ERROR_INVALID_KEY;

        // 2 - Parse value
        JsonVariant value;
        if (!parseAnythingTo(&value)) goto ERROR_INVALID_VALUE;
        if (!object.set(key, value)) goto ERROR_NO_MEMORY;
    }

    SUCCESS_EMPTY_OBJECT:
    SUCCESS_NON_EMPTY_OBJECT:
    return object;

    ERROR_INVALID_KEY:
    ERROR_INVALID_VALUE:
    ERROR_MISSING_BRACE:
    ERROR_MISSING_COLON:
    ERROR_MISSING_COMMA:
    ERROR_NO_MEMORY:
    return JsonObject::invalid();
}

template<typename TReader, typename TWriter>
inline bool ArduinoJson::Internals::JsonBinaryParser<TReader, TWriter>::parseObjectTo(
        JsonVariant *destination) {
    JsonObject &object = parseObject();
    if (!object.success()) return false;

    *destination = object;
    return true;
}

template<typename TReader, typename TWriter>
inline const char *
ArduinoJson::Internals::JsonBinaryParser<TReader, TWriter>::parseString() {
    typename RemoveReference<TWriter>::type::String str = _writer.startString();


    uint32_t strSize = 0;
    uint8_t stringType = _reader.current();
    uint8_t isString=0;


    union {
        uint8_t bytes[4];
        uint32_t value;
    } convertSize;

    convertSize.value=0;

    if (stringType >= MESSAGEPACK_STRING_FIX_MIN && stringType <= MESSAGEPACK_STRING_FIX_MAX) {
        strSize = stringType - MESSAGEPACK_STRING_FIX_MIN;
        _reader.move();
        if (strSize == 0) return NULL;
    } else if (((int8_t) stringType >= MESSAGEPACK_INT_S5_MIN) && ((int8_t) stringType <= MESSAGEPACK_INT_U7_MAX)) {
        strSize = 0;
        str.append(stringType);
        _reader.move();
    } else {
        _reader.move();
        switch (stringType) {
            case MESSAGEPACK_INT_U8:
            case MESSAGEPACK_INT_S8:
                strSize = sizeof(uint8_t);
                goto MSGPACK_MOUNT_STR;
            case MESSAGEPACK_INT_U16:
            case MESSAGEPACK_INT_S16:
                strSize = sizeof(uint16_t);
                goto MSGPACK_MOUNT_STR;
            case MESSAGEPACK_INT_U32:
            case MESSAGEPACK_INT_S32:
            case MESSAGEPACK_FLOAT_32:
                strSize = sizeof(uint32_t);
                goto MSGPACK_MOUNT_STR;
            case MESSAGEPACK_INT_U64:
            case MESSAGEPACK_INT_S64:
            case MESSAGEPACK_FLOAT_64:
                strSize = sizeof(uint64_t);
                goto MSGPACK_MOUNT_STR;
            case MESSAGEPACK_STRING_8:
                strSize = sizeof(uint8_t);
                isString = 1;
                goto MSGPACK_MOUNT_STR;
            case MESSAGEPACK_STRING_16:
                strSize = sizeof(uint16_t);
                isString = 1;
                goto MSGPACK_MOUNT_STR;
            case MESSAGEPACK_STRING_32:
                strSize = sizeof(uint32_t);
                isString = 1;
            MSGPACK_MOUNT_STR:

                if(isString == 0) str.append(stringType);

                while(strSize)
                {
                    strSize--;
                    if (isString == 1) convertSize.bytes[strSize] = _reader.current();
                    else str.append(_reader.current());
                    _reader.move();
                }

                if (isString == 1) strSize = convertSize.value;

                break;

            case MESSAGEPACK_TRUE:
                str.append(1);
                break;

            case MESSAGEPACK_FALSE:
                str.append(0);
                break;

            case MESSAGEPACK_NULL:
                str.append(MESSAGEPACK_NULL);
                break;
        }
    }

    while (strSize--) {

        str.append(_reader.current());
        _reader.move();
    }

    return str.c_str();
}

template<typename TReader, typename TWriter>
inline bool ArduinoJson::Internals::JsonBinaryParser<TReader, TWriter>::parseStringTo(
        JsonVariant *destination) {
    bool hasQuotes = isQuote(_reader.current());
    const char *value = parseString();
    if (value == NULL) return false;
    if (hasQuotes) {
        *destination = value;
    } else {
        *destination = RawJson(value);
    }
    return true;
}
