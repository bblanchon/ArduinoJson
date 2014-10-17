/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include "ArduinoJson/Internals/EscapedString.h"

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

static inline size_t printCharTo(char c, Print* p)
{
    char specialChar = getSpecialChar(c);

    return specialChar != 0
        ? p->write('\\') + p->write(specialChar)
        : p->write(c);
}

size_t EscapedString::printTo(const char* s, Print* p)
{
    if (!s) return p->print("null");
    
    size_t n = p->write('\"');

    while (*s)
    {
        n += printCharTo(*s++, p);
    }

    return n + p->write('\"');
}

static char unescapeChar(char c)
{
    // Optimized for code size on a 8-bit AVR

    const char* p = "b\bf\fn\nr\rt\t";

    while (true)
    {
        if (p[0] == 0) return c;
        if (p[0] == c) return p[1];
        p += 2;
    }
}

char* EscapedString::extractFrom(char* input, char** end)
{
    char* start = input + 1; // skip quote
    char* readPtr = start;
    char* writePtr = start;
    char c;

    do
    {
        c = *readPtr++;

        if (c == '\"')
            break;

        if (c == '\\')
        {
            c = unescapeChar(*readPtr++);
        }

        *writePtr++ = c;
    } while (c != 0);

    *writePtr = 0;

    *end = readPtr;

    return start;
}