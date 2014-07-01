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
        content.string = value;
    }

    JsonValue(double value)
        : implementation(&JsonValue::printNumberTo)
    {
        content.number = value;
    }

    JsonValue(bool value)
        : implementation(&JsonValue::printBooleanTo)
    {
        content.boolean = value;
    }

    JsonValue(Printable& value)
        : implementation(&JsonValue::printObjectTo)
    {
        content.object = &value;
    }

    virtual size_t printTo(Print& p) const
    {
        // handmade polymorphism
        return (this->*implementation)(p);
    }
    
private:
    
    union Content
    {
        bool        boolean;
        double      number;
        Printable*  object;
        const char* string;
    };

    Content content;

    size_t(JsonValue::*implementation)(Print& p)const;

    size_t printBooleanTo(Print& p) const;
    size_t printNumberTo(Print& p) const;
    size_t printObjectTo(Print& p) const;
    size_t printStringTo(Print& p) const;
};