/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonObjectBase.h"

using namespace ArduinoJson::Generator;
using namespace ArduinoJson::Internals;

JsonValue JsonObjectBase::nullValue;

size_t JsonObjectBase::printTo(Print& p) const
{
    size_t n = 0;

    n += p.write('{');

    // NB: the code has been optimized for a small size on a 8-bit AVR

    const KeyValuePair* current = items;
    for (int i = count; i > 0; i--)
    {       
        n += EscapedString::printTo(current->key, p);
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

JsonValue& JsonObjectBase::operator[](char const* key)
{
    KeyValuePair* p = items;

    for (int i = count; i > 0; --i)
    {
        if (p->matches(key))
            return p->value;

        p++;
    }

    JsonValue* value;

    if (count < capacity)
    {
        count++;
        p->key = key;
        value = &p->value;
    }
    else
    {
        value = &nullValue;
    }

    value->reset();
    return *value;
}

bool JsonObjectBase::containsKey(char const* key) const
{
    KeyValuePair* p = items;

    for (int i = count; i > 0; --i)
    {
        if (p->matches(key))
            return true;

        p++;
    }

    return false;
}