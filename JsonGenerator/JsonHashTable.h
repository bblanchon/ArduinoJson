/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "JsonObjectBase.h"

template<int N>
class JsonHashTable : public JsonObjectBase
{
public:

    JsonHashTable()
    {
        itemCount = 0;
    }

    template<typename T>
    void add(const char* key, T value)
    {
        add(key, JsonValue(value));
    }

    void add(const char* key, JsonValue value)
    {
        if (itemCount >= N) return;

        items[itemCount].key = key;
        items[itemCount].value = value;
        itemCount++;
    }

    using JsonObjectBase::printTo;
    
private:

    struct KeyValuePair
    {
        const char* key;
        JsonValue value;
    };

    KeyValuePair items[N];
    int itemCount;

    virtual size_t printTo(Print& p) const
    {
        size_t n = 0;

        n += p.write('{');

        for (int i = 0; i < itemCount; i++)
        {
            JsonValue key(items[i].key);

            if (i > 0)
            {
                n += p.write(',');
            }

            n += key.printTo(p);
            n += p.write(':');
            n += items[i].value.printTo(p);
        }

        n += p.write('}');

        return n;
    }
};

