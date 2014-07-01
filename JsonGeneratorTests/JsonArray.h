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

    template<typename T>
    void add(T value)
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

    virtual size_t writeTo(JsonSink& sb)
    {
        size_t n = 0;
        
        n += sb.write("[");

        for (int i = 0; i < itemCount; i++)
        {
            if (i > 0)
            {
                n += sb.write(",");
            }

            n += items[i].writeTo(sb);
        }

        n += sb.write("]");

        return n;
    }
};

