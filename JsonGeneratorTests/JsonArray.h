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

    virtual void writeTo(JsonSink& sb)
    {
        sb.append("[");
        sb.reserveRoom(1);

        for (int i = 0; i < itemCount; i++)
        {
            if (i>0) sb.append(",");
            items[i].writeTo(sb);
        }

        sb.releaseRoom(1);
        sb.append("]");
    }
};

