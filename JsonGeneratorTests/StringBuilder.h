/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include <cstdarg>
#include <cstdio>
#include <cstring>

class StringBuilder
{
public:
    StringBuilder(char* buf, size_t size)
        : buffer(buf), capacity(size-1), length(0)
    {
        buffer[0] = 0;
    }

    void append(const char* s);
    void appendEscaped(const char* s);
    void appendFormatted(const char* format, ...);

private:
    char* buffer;
    size_t capacity;
    size_t length;
};

