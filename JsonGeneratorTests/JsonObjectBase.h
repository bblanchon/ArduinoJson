/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "StringBuilder.h"

class JsonObjectBase
{
public:

    void writeTo(char* buffer, size_t bufferSize)
    {
        StringBuilder sb(buffer, bufferSize);
        writeTo(sb);
    }

protected:

    enum JsonValueType
    {
        JSON_STRING,
        JSON_NUMBER,
        JSON_BOOLEAN,
        JSON_OBJECT,
    };

    union JsonValueContent
    {
        const char*     string;
        double          number;
        bool            boolean;
        JsonObjectBase* object;
    };

    struct JsonValue
    {
        JsonValueType    type;
        JsonValueContent content;
    };

    void writeValueTo(JsonValue& obj, StringBuilder& sb);

    virtual void writeTo(StringBuilder& sb) = 0;
};

