/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

class JsonSink
{
public:

    virtual size_t append(char c) = 0;
    
    size_t append(const char* s)
    {
        size_t n = 0;
        while (*s)
        {
            n += append(*s++);
        }
        return n;
    }
};

