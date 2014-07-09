/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include "EscapedString.h"

using namespace ArduinoJson::Internals;

static inline char getSpecialChar(char c)
{
    // Optimized for code size on a 8-bit AVR

    const char* specials = "\"\"\\\\\bb\ff\nn\rr\tt";
    
    while (true)
    {
        if (specials[0] == 0)
            return 0;

        if (specials[0] == c)
            return specials[1];

        specials += 2;
    }
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