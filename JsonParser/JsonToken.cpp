/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonToken.h"

using namespace ArduinoJson::Parser;

char* JsonToken::getText()
{
    char* s = _json + _token->start;
    _json[_token->end] = 0;

    unescapeString(s);

    return s;
}

inline void JsonToken::unescapeString(char* s)
{
    char* readPtr = s;
    char* writePtr = s;
    char c;

    do
    {
        c = *readPtr++;

        if (c == '\\')
        {
            c = unescapeChar(*readPtr++);
        }

        *writePtr++ = c;

    } while (c != 0);
}

inline char JsonToken::unescapeChar(char c)
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

JsonToken JsonToken::nextSibling() const
{
    // start with current token
    jsmntok_t* t = _token;

    // count the number of token to skip
    int yetToVisit = 1;
    
    // skip all nested tokens
    while (yetToVisit)
    {
        yetToVisit += t->size - 1;
        t++;
    }

    // build a JsonToken at the new location
    return JsonToken(_json, t);
}
