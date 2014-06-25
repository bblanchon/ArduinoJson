/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "StringBuilder.h"

enum JsonObjectType
{
    JSON_STRING,
    JSON_NUMBER,
    JSON_BOOLEAN,
};

union JsonObjectValue
{
    const char* string;
    double      number;
    bool        boolean;
};

struct JsonObject
{
    JsonObjectType type;
    JsonObjectValue value;
};

template<int N>
class JsonArray
{
    

public:
    JsonArray()
    {
        itemCount = 0;
    }
    
    void add(const char* value)
    {
        JsonObjectValue v;
        v.string = value;
        addItem(JSON_STRING, v);
    }

    void add(double value)
    {
        JsonObjectValue v;
        v.number = value;
        addItem(JSON_NUMBER, v);
    }

    void add(bool value)
    {
        JsonObjectValue v;
        v.boolean = value;
        addItem(JSON_BOOLEAN, v);
    }

    void writeTo(char* buffer, size_t bufferSize)
    {
        StringBuilder sb(buffer, bufferSize);
        writeTo(sb);
    }

private:
    JsonObject items[N];
    int itemCount;

    void writeTo(StringBuilder& sb)
    {
        sb.append("[");

        for (int i = 0; i < itemCount; i++)
        {
            if (i>0)
                sb.append(",");

            JsonObjectValue value = items[i].value;

            switch (items[i].type)
            {
            case JSON_STRING:
                if (value.string)
                    sb.append("\"%s\"", value.string);
                else
                    sb.append("null");
                break;

            case JSON_NUMBER:
                sb.append("%lg", value.number);
                break;

            case JSON_BOOLEAN:
                sb.append(value.boolean ? "true" : "false");
                break;
            }
        }

        sb.append("]");
    }

    void addItem(JsonObjectType type, JsonObjectValue value)
    {
        if (itemCount >= N) return;

        items[itemCount].type = type;
        items[itemCount].value = value;
        itemCount++;
    }
};

