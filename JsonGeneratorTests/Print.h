/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

class Print
{
public:

    virtual size_t write(char c) = 0;
    
    size_t write(const char* s)
    {
        size_t n = 0;
        while (*s)
        {
            n += write(*s++);
        }
        return n;
    }
};

