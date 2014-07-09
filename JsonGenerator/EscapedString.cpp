/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include "EscapedString.h"

using namespace ArduinoJson::Internals;

static inline char getSpecialChar(char c)
{
    switch (c)
    {
    case '"':
        return '"';

    case '\\':
        return '\\';

    case '\b':
        return 'b';

    case '\f':
        return 'f';

    case '\n':
        return 'n';

    case '\r':
        return 'r';

    case '\t':
        return 't';

    default:
        return 0;
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