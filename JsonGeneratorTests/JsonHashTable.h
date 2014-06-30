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

    using JsonObjectBase::writeTo;
    
private:

    struct KeyValuePair
    {
        const char* key;
        JsonValue value;
    };

    KeyValuePair items[N];
    int itemCount;

    virtual void writeTo(JsonSink& sink)
    {
        sink.append("{");
        sink.reserveRoom(1);

        for (int i = 0; i < itemCount; i++)
        {
            if (i>0) sink.append(",");

            JsonValue key(items[i].key);

            key.writeTo(sink);
            sink.append(":");
            items[i].value.writeTo(sink);
        }

        sink.releaseRoom(1);
        sink.append("}");
    }
};

