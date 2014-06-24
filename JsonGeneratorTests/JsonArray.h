/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include <stdarg.h>

enum JsonObjectType
{
    JSON_STRING,
    JSON_NUMBER,
};

union JsonObjectValue
{
    const char* string;
    double number;
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

    void writeTo(char* buffer, size_t bufferSize)
    {
        buffer[0] = 0;

        append(buffer, bufferSize, "[");

        for (int i = 0; i < itemCount; i++)
        {
            if (i>0)
                append(buffer, bufferSize, ",");

            switch (items[i].type)
            {
            case JSON_STRING:
                append(buffer, bufferSize, "'%s'", items[i].value.string);
                break;

            case JSON_NUMBER:
                append(buffer, bufferSize, "%lg", items[i].value.number);
                break;
            }
        }

        append(buffer, bufferSize, "]");
    }

private:
    JsonObject items[N];
    int itemCount;

    void append(char* dest, size_t destSize, const char* format, ...)
    {
        int len = strlen(dest);

        va_list args;
        va_start(args, format);
        vsnprintf(dest + len, destSize - len, format, args);
        va_end(args);
    }
};

