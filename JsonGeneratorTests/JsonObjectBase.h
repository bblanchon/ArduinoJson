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

    void writeObjectTo(ObjectContainer& obj, StringBuilder& sb)
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

        case JSON_OBJECT:
            if (obj.value.object)
                obj.value.object->writeTo(sb);
            else
                sb.append("null");
            break;
        }
    }
};

