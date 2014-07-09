/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include "EscapedString.h"

using namespace ArduinoJson::Internals;

static inline char getSpecialChar(char c)
{
    // Optimized for code size on a 8-bit AVR

    const char*p = "\"\"\\\\\bb\ff\nn\rr\tt\0";

    while (p[0] && p[0] != c)
    {
        p += 2;
    }
    
    return p[1];
}

size_t EscapedString::printTo(Print& p) const
{
    const char* s = rawString;

    if (!s)
    {
        return p.print("null");
    }

    size_t n = 0;

    n += p.write('\"');

    while (*s)
    {
        char specialChar = getSpecialChar(*s);

        if (specialChar)
        {
            n += p.write('\\');
            n += p.write(specialChar);
        }
        else
        {
            n += p.write(*s);
        }

        s++;        
    }

    n += p.write('\"');

    return n;
}