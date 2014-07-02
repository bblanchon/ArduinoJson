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

    JsonValue(const char* value)
        : implementation(&JsonValue::printStringTo)
    {
        content.asString = value;
    }

    JsonValue(double value)
        : implementation(&JsonValue::printNumberTo)
    {
        content.asDouble = value;
    }

    JsonValue(bool value)
        : implementation(&JsonValue::printBooleanTo)
    {
        content.asBool = value;
    }

    JsonValue(Printable& value)
        : implementation(&JsonValue::printObjectTo)
    {
        content.asPrintable = &value;
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
        Printable*  asPrintable;
        const char* asString;
    };

    Content content;

    size_t(JsonValue::*implementation)(Print& p)const;

    size_t printBooleanTo(Print& p) const;
    size_t printNumberTo(Print& p) const;
    size_t printObjectTo(Print& p) const;
    size_t printStringTo(Print& p) const;
};