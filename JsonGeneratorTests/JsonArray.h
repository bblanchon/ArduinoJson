/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "JsonObjectBase.h"
#include "StringBuilder.h"

template<int N>
class JsonArray : public JsonObjectBase
{
public:
    JsonArray()
    {
        itemCount = 0;
    }
    
    void add(const char* value)
    {
        add(JsonValue(value));
    }

    void add(double value)
    {
        add(JsonValue(value));
    }

    void add(bool value)
    {
        add(JsonValue(value));
    }

    void add(JsonObjectBase& value)
    {
        add(JsonValue(value));
    }

    void add(JsonValue value)
    {
        if (itemCount >= N) return;

        items[itemCount] = value;
        itemCount++;
    }

    using JsonObjectBase::writeTo;

private:
    JsonValue items[N];
    int itemCount;

    virtual void writeTo(StringBuilder& sb)
    {
        sb.append("[");

        for (int i = 0; i < itemCount; i++)
        {
            if (i>0) sb.append(",");
            items[i].writeTo(sb);
        }

        sb.append("]");
    }
};

