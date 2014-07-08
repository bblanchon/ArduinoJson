#include "JsonHashTable.h"

using namespace ArduinoJson::Generator;

size_t JsonHashTableBase::printTo(Print& p) const
{
    size_t n = 0;

    n += p.write('{');

    KeyValuePair* current = items;
    for (int i = count; i > 0; i--)
    {       
        n += current->key.printTo(p);
        n += p.write(':');
        n += current->value.printTo(p);

        current++;

        if (i > 1)
        {
            n += p.write(',');
        }
    }

    n += p.write('}');

    return n;
}