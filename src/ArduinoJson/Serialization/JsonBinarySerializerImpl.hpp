// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../JsonArray.hpp"
#include "../JsonArraySubscript.hpp"
#include "../JsonObject.hpp"
#include "../JsonObjectSubscript.hpp"
#include "../JsonVariant.hpp"
#include "JsonSerializer.hpp"



//TODO: Binary Serializer with MessagePack (Entire set)

template<typename Writer>
inline void ArduinoJson::Internals::JsonSerializer<Writer>::binarySerialize(
        const JsonArray &array, Writer &writer) {
    writer.beginMessagepackArray(array.size());

    JsonArray::const_iterator it = array.begin();
    while (it != array.end()) {
        binarySerialize(*it, writer);
        ++it;
        if (it == array.end()) break;
    }

}

template<typename Writer>
inline void ArduinoJson::Internals::JsonSerializer<Writer>::binarySerialize(
        const JsonArraySubscript &arraySubscript, Writer &writer) {
    binarySerialize(arraySubscript.as<JsonVariant>(), writer);
}

template<typename Writer>
inline void ArduinoJson::Internals::JsonSerializer<Writer>::binarySerialize(
        const JsonObject &object, Writer &writer) {

    writer.beginMessagepackObject(object.size());

    JsonObject::const_iterator it = object.begin();
    while (it != object.end()) {
        writer.writeMessagepackString(it->key);

        binarySerialize(it->value, writer);

        ++it;
        if (it == object.end()) break;
    }

}

template<typename Writer>
template<typename TKey>
inline void ArduinoJson::Internals::JsonSerializer<Writer>::binarySerialize(
        const JsonObjectSubscript <TKey> &objectSubscript, Writer &writer) {
    binarySerialize(objectSubscript.template as<JsonVariant>(), writer);
}

template<typename Writer>
inline void ArduinoJson::Internals::JsonSerializer<Writer>::binarySerialize(
        const JsonVariant &variant, Writer &writer) {
    switch (variant._type) {
        case JSON_FLOAT:
            writer.writeMessagepackFloat(variant._content.asFloat);
            return;

        case JSON_ARRAY:
            binarySerialize(*variant._content.asArray, writer);
            return;

        case JSON_OBJECT:
            binarySerialize(*variant._content.asObject, writer);
            return;

        case JSON_STRING:
            writer.writeMessagepackString(variant._content.asString);
            return;

        case JSON_UNPARSED:
            writer.writeRaw(variant._content.asString);
            return;

        case JSON_NEGATIVE_INTEGER:

            switch (sizeof(variant._content.asInteger)) //get positive value and turn it negative with correct cast
            {
                case sizeof(int8_t):
                    writer.writeMessagepackInteger(~((int8_t)variant._content.asInteger)+1);
                    break;
                case sizeof(int16_t):
                    writer.writeMessagepackInteger(~((int16_t)variant._content.asInteger)+1);
                    break;
                case sizeof(int32_t):
                    writer.writeMessagepackInteger(~((int32_t)variant._content.asInteger)+1);
                    break;
                case sizeof(int64_t):
                    writer.writeMessagepackInteger(~((int64_t)variant._content.asInteger)+1);
                    break;
            }

            return;

        case JSON_POSITIVE_INTEGER:
            writer.writeMessagepackInteger(variant._content.asInteger);
            return;

        case JSON_BOOLEAN:
            writer.writeMessagepackBoolean(variant._content.asInteger != 0);
            return;

        default:  // JSON_UNDEFINED
            return;
    }
}