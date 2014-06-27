/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "StringBuilder.h"

class JsonObjectBase;

class JsonValue
{    
public:

    JsonValue()
    {
    }

    JsonValue(const char* value)
        : type(JSON_STRING)
    {
        content.string = value;
    }

    JsonValue(double value)
        : type(JSON_NUMBER)
    {
        content.number = value;
    }

    JsonValue(bool value)
        : type(JSON_BOOLEAN)
    {
        content.boolean = value;
    }

    JsonValue(JsonObjectBase& value)
        : type(JSON_OBJECT)
    {
        content.object = &value;
    }

    void writeTo(StringBuilder& sb);

private:

    enum Type
    {
        JSON_STRING,
        JSON_NUMBER,
        JSON_BOOLEAN,
        JSON_OBJECT,
    };

    union Content
    {
        const char*     string;
        double          number;
        bool            boolean;
        JsonObjectBase* object;
    };

    Type    type;
    Content content;
};

