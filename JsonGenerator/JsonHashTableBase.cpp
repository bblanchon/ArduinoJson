#include "JsonHashTable.h"

using namespace ArduinoJson::Generator;

size_t JsonHashTableBase::printTo(Print& p) const
{
    size_t n = 0;

    n += p.write('{');

    for (int i = 0; i < count; i++)
    {
        if (i > 0)
        {
            n += p.write(',');
        }

        n += items[i].key.printTo(p);
        n += p.write(':');
        n += items[i].value.printTo(p);
    }

    n += p.write('}');

    return n;
}