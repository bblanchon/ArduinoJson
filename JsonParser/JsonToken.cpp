
#include "JsonToken.h"

using namespace ArduinoJson::Internal;

int JsonToken::nestedTokenCount() const
{
    jsmntok_t* t = token;
    int yetToVisit = t->size;
    int count = 0;

    while (yetToVisit)
    {
        count++;
        t++;
        yetToVisit--;
        yetToVisit += t->size;
    }

    return count;
}