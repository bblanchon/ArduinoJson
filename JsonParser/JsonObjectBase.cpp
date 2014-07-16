/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonObjectBase.h"

using namespace ArduinoJson::Parser;

int JsonObjectBase::getNestedTokenCount(jsmntok_t* token)
{
    int tokensToVisit = token->size;
    int count = 0;

    while (tokensToVisit)
    {
        count++;
        token++;
        tokensToVisit--;
        tokensToVisit += token->size;
    }

    return count;
}