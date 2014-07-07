/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include "EscapedString.h"
#include "JsonValue.h"

using namespace ArduinoJson::Generator;

size_t JsonValue::printBoolTo(const Content& c, Print& p)
{
    return p.print(c.asBool ? "true" : "false");
}

size_t JsonValue::printLongTo(const Content& c, Print& p)
{
    return p.print(c.asLong);
}

size_t JsonValue::printPrintableTo(const Content& c, Print& p)
{
    if (c.asPrintable)
        return ((Printable*) c.asPrintable)->printTo(p);
    else
        return p.print("null");
}

size_t JsonValue::printStringTo(const Content& c, Print& p)
{
    EscapedString s(c.asString);
    return s.printTo(p);
}