/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonArrayBase.h"

using namespace ArduinoJson::Generator;
using namespace ArduinoJson::Internals;

size_t JsonArrayBase::printTo(Print& p) const
{
    size_t n = 0;

    n += p.write('[');

    // NB: the code has been optimized for a small size on a 8-bit AVR

    const JsonValue* current = _items;
    for (int i = _count; i > 0; i--)
    {       
        n += current->printTo(p);
        current++;

        if (i > 1)
        {
            n += p.write(',');
        }
    }

    n += p.write(']');

    return n;
}
