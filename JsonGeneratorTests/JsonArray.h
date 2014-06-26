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
        ObjectValue v;
        v.string = value;
        addItem(JSON_STRING, v);
    }

    void add(double value)
    {
        ObjectValue v;
        v.number = value;
        addItem(JSON_NUMBER, v);
    }

    void add(bool value)
    {
        ObjectValue v;
        v.boolean = value;
        addItem(JSON_BOOLEAN, v);
    }

    void add(JsonObjectBase& value)
    {
        ObjectValue v;
        v.object = &value;
        addItem(JSON_OBJECT, v);
    }

    using JsonObjectBase::writeTo;

private:
    ObjectContainer items[N];
    int itemCount;

    virtual void writeTo(StringBuilder& sb)
    {
        sb.append("[");

        for (int i = 0; i < itemCount; i++)
        {
            if (i>0) sb.append(",");
            writeObjectTo(items[i], sb);
        }

        sb.append("]");
    }

    void addItem(ObjectType type, ObjectValue value)
    {
        if (itemCount >= N) return;

        items[itemCount].type = type;
        items[itemCount].value = value;
        itemCount++;
    }
};

