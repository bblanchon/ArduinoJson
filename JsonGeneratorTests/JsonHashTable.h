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

    void add(const char* key, const char* value)
    {
        addItem(key, JsonValue(value));
    }

    void addItem(const char* key, JsonValue value)
    {
        if (itemCount >= N) return;

        items[itemCount].key = key;
        items[itemCount].value = value;
        itemCount++;
    }

    using JsonObjectBase::writeTo;
    
private:

    struct KeyValuePair
    {
        const char* key;
        JsonValue value;
    };

    KeyValuePair items[N];
    int itemCount;

    virtual void writeTo(StringBuilder& sb)
    {
        sb.append("{");

        for (int i = 0; i < itemCount; i++)
        {
            if (i>0) sb.append(",");
            sb.appendEscaped(items[i].key);
            sb.append(":");
            items[i].value.writeTo(sb);
        }

        sb.append("}");
    }
};

