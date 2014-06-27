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
        JsonValueContent v;
        v.string = value;
        addItem(JSON_STRING, v);
    }

    void add(double value)
    {
        JsonValueContent v;
        v.number = value;
        addItem(JSON_NUMBER, v);
    }

    void add(bool value)
    {
        JsonValueContent v;
        v.boolean = value;
        addItem(JSON_BOOLEAN, v);
    }

    void add(JsonObjectBase& value)
    {
        JsonValueContent v;
        v.object = &value;
        addItem(JSON_OBJECT, v);
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
            writeValueTo(items[i], sb);
        }

        sb.append("]");
    }

    void addItem(JsonValueType type, JsonValueContent content)
    {
        if (itemCount >= N) return;

        items[itemCount].type = type;
        items[itemCount].content = content;
        itemCount++;
    }
};

