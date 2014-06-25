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
        if (itemCount >= N) return;

        items[itemCount].type = JSON_STRING;
        items[itemCount].value.string = value;
        itemCount++;
    }

    void add(double value)
    {
        if (itemCount >= N) return;
        
        items[itemCount].type = JSON_NUMBER;
        items[itemCount].value.number = value;
        itemCount++;
    }

    void add(bool value)
    {
        if (itemCount >= N) return;

        items[itemCount].type = JSON_BOOLEAN;
        items[itemCount].value.boolean = value;
        itemCount++;
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
};

