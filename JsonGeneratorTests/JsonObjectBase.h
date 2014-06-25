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

    enum ObjectType
    {
        JSON_STRING,
        JSON_NUMBER,
        JSON_BOOLEAN,
        JSON_OBJECT,
    };

    union ObjectValue
    {
        const char*     string;
        double          number;
        bool            boolean;
        JsonObjectBase* object;
    };

    struct ObjectContainer
    {
        ObjectType type;
        ObjectValue value;
    };

    void writeObjectTo(ObjectContainer& obj, StringBuilder& sb);

    virtual void writeTo(StringBuilder& sb) = 0;
};

