/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

template<int N>
class JsonArray
{
public:
    JsonArray()
    {
        itemCount = 0;
    }
    
    void add(const char* data)
    {
        if (itemCount <= N)
            items[itemCount++] = data;
    }

    void writeTo(char* buffer, size_t bufferSize)
    {
        buffer[0] = 0;

        append("[", buffer, bufferSize);

        for (int i = 0; i < itemCount; i++)
        {
            if (i>0)
                append(",", buffer, bufferSize);

            append("'", buffer, bufferSize);
            append(items[i], buffer, bufferSize);           
            append("'", buffer, bufferSize);
        }

        append("]", buffer, bufferSize);       
    }

private:
    const char* items[N];
    int itemCount;

    void append(const char* source, char* dest, size_t destSize)
    {
        int len = strlen(dest);
        strncpy(dest + len, source, destSize - len);
    }
};

