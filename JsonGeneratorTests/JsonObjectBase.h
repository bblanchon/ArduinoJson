/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "StringBuilder.h"

class JsonObjectBase
{
public:


protected:

    virtual void writeTo(StringBuilder& sb) = 0;

    enum JsonObjectType
    {
        JSON_STRING,
        JSON_NUMBER,
        JSON_BOOLEAN,
    };

    union JsonObjectValue
    {
        const char* string;
        double      number;
        bool        boolean;
    };

    struct JsonObject
    {
        JsonObjectType type;
        JsonObjectValue value;
    };

    void writeObjectTo(const JsonObject& obj, StringBuilder& sb)
    {
        switch (obj.type)
        {
        case JSON_STRING:
            if (obj.value.string)
                sb.append("\"%s\"", obj.value.string);
            else
                sb.append("null");
            break;

        case JSON_NUMBER:
            sb.append("%lg", obj.value.number);
            break;

        case JSON_BOOLEAN:
            sb.append(obj.value.boolean ? "true" : "false");
            break;
        }
    }
};

