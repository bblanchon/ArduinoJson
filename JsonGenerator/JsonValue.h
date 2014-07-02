/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "Printable.h"
#include "StringBuilder.h"

class JsonValue : public Printable
{    
public:

    JsonValue()
    {
    }

    JsonValue(bool value)
        : implementation(&JsonValue::printBoolTo)
    {
        content.asBool = value;
    }

    JsonValue(double value)
        : implementation(&JsonValue::printDoubleTo)
    {
        content.asDouble = value;
    }

    JsonValue(long value)
        : implementation(&JsonValue::printLongTo)
    {
        content.asLong = value;
    }

    JsonValue(Printable& value)
        : implementation(&JsonValue::printPrintableTo)
    {
        content.asPrintable = &value;
    }

    JsonValue(const char* value)
        : implementation(&JsonValue::printStringTo)
    {
        content.asString = value;
    }

    virtual size_t printTo(Print& p) const
    {
        // handmade polymorphism
        return (this->*implementation)(p);
    }
    
private:
    
    union Content
    {
        bool        asBool;
        double      asDouble;
        long        asLong;
        Printable*  asPrintable;
        const char* asString;
    };

    Content content;

    size_t(JsonValue::*implementation)(Print& p)const;

    size_t printBoolTo(Print& p) const;
    size_t printDoubleTo(Print& p) const;
    size_t printLongTo(Print& p) const;
    size_t printPrintableTo(Print& p) const;
    size_t printStringTo(Print& p) const;
};