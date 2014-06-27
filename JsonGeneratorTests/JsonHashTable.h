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
        JsonValueContent v;
        v.string = value;
        addItem(key, JSON_STRING, v);
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
            writeValueTo(items[i].value, sb);
        }

        sb.append("}");
    }

    void addItem(const char* key, JsonValueType type, JsonValueContent content)
    {
        if (itemCount >= N) return;

        items[itemCount].key = key;
        items[itemCount].value.type = type;
        items[itemCount].value.content = content;
        itemCount++;
    }
};

