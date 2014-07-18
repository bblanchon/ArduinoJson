
#include "JsonToken.h"

using namespace ArduinoJson::Internal;

JsonToken JsonToken::nextSibling() const
{
    jsmntok_t* t = token;
    int yetToVisit = t->size;

    while (yetToVisit)
    {
        t++;
        yetToVisit--;
        yetToVisit += t->size;
    }

    return t + 1;
}