/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonToken.h"

using namespace ArduinoJson::Parser;

static char unescapeChar(char c)
{
    switch (c)
    {
    case 'b': return '\b';
    case 'f': return '\f';
    case 'n': return '\n';
    case 'r': return '\r';
    case 't': return '\t';
    default: return c;
    }
}

static void unescapeString(char* s)
{
    char* readPtr = s;
    char* writePtr = s;

    while (true)
    {
        if (*readPtr == '\\')
        {
            readPtr++;
            *writePtr = unescapeChar(*readPtr);
        }
        else
        {
            *writePtr = *readPtr;
        }

        if (*writePtr == 0) break;

        readPtr++;
        writePtr++;
    }
}

char* JsonToken::getText()
{
    char* s = json + token->start;
    json[token->end] = 0;

    unescapeString(s);

    return s;
}

JsonToken JsonToken::nextSibling() const
{
    // start with current token
    jsmntok_t* t = token;

    // count the number of token to skip
    int yetToVisit = 1;
    
    // skip all nested tokens
    while (yetToVisit)
    {
        yetToVisit += t->size - 1;
        t++;
    }

    // build a JsonToken at the new location
    return JsonToken(json, t);
}