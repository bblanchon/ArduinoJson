/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include "EscapedString.h"

using namespace ArduinoJson::Internals;

static inline char getSpecialChar(char c)
{
    // Optimized for code size on a 8-bit AVR

    const char* p = "\"\"\\\\\bb\ff\nn\rr\tt\0";

    while (p[0] && p[0] != c)
    {
        p += 2;
    }

    return p[1];
}

static inline size_t printCharTo(char c, Print& p)
{
    char specialChar = getSpecialChar(c);

    return specialChar != 0
        ? p.write('\\') + p.write(specialChar)
        : p.write(c);
}

size_t EscapedString::printTo(const char* s, Print& p)
{
    if (!s) return p.print("null");
    
    size_t n = p.write('\"');

    while (*s)
    {
        n += printCharTo(*s++, p);
    }

    return n + p.write('\"');
}
