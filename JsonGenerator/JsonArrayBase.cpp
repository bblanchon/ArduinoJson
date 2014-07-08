/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonArrayBase.h"

using namespace ArduinoJson::Generator;

size_t JsonArrayBase::printTo(Print& p) const
{
    size_t n = 0;

    n += p.write('[');

    for (int i = 0; i < count; i++)
    {
        if (i > 0)
        {
            n += p.write(',');
        }

        n += items[i].printTo(p);
    }

    n += p.write(']');

    return n;
}