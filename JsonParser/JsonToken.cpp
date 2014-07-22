/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonToken.h"

using namespace ArduinoJson::Parser;

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