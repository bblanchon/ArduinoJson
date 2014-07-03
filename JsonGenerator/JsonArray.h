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

    void add(double value, int digits=2)
    {
        add(JsonValue(value, digits));
    }

    void add(JsonValue value)
    {
        if (itemCount >= N) return;

        items[itemCount] = value;
        itemCount++;
    }

    using JsonObjectBase::printTo;

private:
    JsonValue items[N];
    int itemCount;

    virtual size_t printTo(Print& p) const
    {
        size_t n = 0;
        
        n += p.write('[');

        for (int i = 0; i < itemCount; i++)
        {
            if (i > 0)
            {
                n += p.write(',');
            }

            n += items[i].printTo(p);
        }

        n += p.write(']');

        return n;
    }
};

